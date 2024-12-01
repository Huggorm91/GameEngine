#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <fstream>

#include "GameServer.h"

#include "NetworkShared/MessageFunctions.h"

#include "GameplayEngine/Engine.h"
#include "GameplayEngine/PostMaster/PostMaster.h"
#include "GameplayEngine/Managers/SceneManager.h"
#include "GameplayEngine/Managers/ObjectManager.h"
#include "GameplayEngine/Network/NetworkManager.h"
#include "GameplayEngine/Network/GameObjectMessage.h"

#include "AssetManager/AssetManager.h"

#include "CrimsonUtilities/Time/Time.h"
#include "CrimsonUtilities/Math/Random.h"

bool GameServer::Initialize()
{
#ifndef _DEBUG
	try
	{
#endif // _DEBUG
		Engine::Init(this);

		myServer.Init(&Engine::GetLogger());
		myServer.SetIncommingMessageCallback(Network::MessageType::Connect, [this](Network::ClientInfo& aClient, const Network::NetMessage&) { HandleConnection(aClient); });
		myServer.SetIncommingMessageCallback(Network::MessageType::CreateGameObject, [this](Network::ClientInfo& aClient, const Network::NetMessage& aMessage) { HandleCreateObject(aClient, aMessage); });
		myServer.SetIncommingMessageCallback(Network::MessageType::DeleteGameObject, [this](Network::ClientInfo& aClient, const Network::NetMessage& aMessage) { HandleDeleteObject(aClient, aMessage); });
		myServer.SetIncommingMessageCallback(Network::MessageType::GameObjectMessage, [this](Network::ClientInfo& aClient, const Network::NetMessage& aMessage) { HandleObjectMessage(aClient, aMessage); });

		AssetManager::Init();
		AssetManager::GeneratePrimitives();
#ifndef _DEBUG
	}
	catch (const std::exception& anException)
	{
		HandleCrash(anException);
		Shutdown();
		return false;
	}
	catch (...)
	{
		HandleCrash(std::invalid_argument("Caught unknown Error!"));
		Shutdown();
		return false;
	}
#endif // _DEBUG
	return true;
}

int GameServer::Run()
{
	Init();
	Crimson::Time::Init();

	bool isRunning = true;
	while (isRunning)
	{
#ifndef _DEBUG
		try
		{
#endif // _DEBUG

			Update();

#ifndef _DEBUG
		}
		catch (const std::exception& anException)
		{
			isRunning = false;
			HandleCrash(anException);
			Shutdown();
			return EXIT_FAILURE;
		}
		catch (...)
		{
			isRunning = false;
			HandleCrash(std::invalid_argument("Caught unknown Error!"));
			Shutdown();
			return EXIT_FAILURE;
		}
#endif // _DEBUG
	}

	Shutdown();

	return EXIT_SUCCESS;
}

void GameServer::Shutdown()
{
	myServer.ShutDown();
}

void GameServer::RecieveMessage(const Crimson::Message& aMessage)
{
	switch (aMessage.GetMessageType())
	{
	case Crimson::eMessageType::Collision_OnCollisionEnter:
	{
		if (aMessage.HasDataType<std::pair<UUIDv4::UUID, UUIDv4::UUID>>())
		{
			const auto& [first, second] = *aMessage.GetDataAsUUIDPair();
			SendDeleteObjectMessage(first);
			SendDeleteObjectMessage(second);
			Engine::GetObjectManager().RemoveGameObjectAtEndOfFrame(first);
			Engine::GetObjectManager().RemoveGameObjectAtEndOfFrame(second);
		}
		break;
	}
	case Crimson::eMessageType::GameObject_Died:
	{
		if (aMessage.HasDataType<UUIDv4::UUID>())
		{
			const auto& id = *aMessage.GetDataAsUUID();
			SendDeleteObjectMessage(id);
			Engine::GetObjectManager().RemoveGameObjectAtEndOfFrame(id);
		}
		break;
	}
	case Crimson::eMessageType::GameObject_Enable:
	{
		if (aMessage.HasDataType<std::pair<UUIDv4::UUID, UUIDv4::UUID>>())
		{
			const auto& ids = *aMessage.GetDataAsUUIDPair();
			SendSetActiveMessage(true, ids.first, ids.second);
		}
		break;
	}
	case Crimson::eMessageType::GameObject_Disable:
	{
		if (aMessage.HasDataType<std::pair<UUIDv4::UUID, UUIDv4::UUID>>())
		{
			const auto& ids = *aMessage.GetDataAsUUIDPair();
			SendSetActiveMessage(false, ids.first, ids.second);
		}
		break;
	}
	default:
		break;
	}
}

void GameServer::SendTransformChanged(const Transform& aTransform, const UUIDv4::UUID& anID)
{
	constexpr rsize_t dataSize = sizeof(Network::GameObjectMessage::data);
	constexpr rsize_t vectorSize = sizeof(Crimson::Vector3f);
	constexpr rsize_t messageSize = vectorSize + vectorSize + sizeof(double);

	Network::GameObjectMessage message;
	message.id = anID;
	message.action = Network::ObjectAction::Move;
	message.size = messageSize;

	// TODO: Rework timestamp to use a timepoint relative to servertime. 
	// Current idea: Server saves time when started and sends this timepoint to all clients that connects.
	// Should probably cache the current timepoint in Update
	double timestamp = Crimson::Time::GetTotalTime();
	constexpr int timestampOffset = vectorSize + vectorSize;
	memcpy_s(message.data, dataSize, &aTransform.GetPosition(), vectorSize);
	memcpy_s(message.data + vectorSize, dataSize - vectorSize, &aTransform.GetRotationRadian(), vectorSize);
	memcpy_s(message.data + timestampOffset, dataSize - timestampOffset, &timestamp, sizeof(double));

	auto netMessage = Network::CreateGameObjectMessage(message);
	myServer.SendMessageToClients(netMessage);
}

void GameServer::HandleCrash(const std::exception& anException)
{
	// Log crash
	Engine::GetLogger().Err("Server has crashed!");
	Engine::GetLogger().Warn("Writing exception to log file!");
	Engine::GetLogger().LogException(anException);
	Engine::GetLogger().PrintHistoryToFile(Network::Server::GetLogfileName());

	// Leave console up to let user read information
	system("PAUSE");
}

void GameServer::Init()
{
	Engine::GetPostMaster().Subscribe(this, Crimson::eMessageType::Collision_OnCollisionEnter);
	Engine::GetPostMaster().Subscribe(this, Crimson::eMessageType::GameObject_Died);

	Engine::GetSceneManager().LoadScene("Test");
}

void GameServer::Update()
{
	Engine::BeginFrame();
	Engine::GetObjectManager().UpdateObjects(false);

	// TODO: Remove
	{ // Part of school assignment
		static float timer = 0.f;
		timer += Crimson::Time::GetDeltaTime();
		if (timer >= 1.f)
		{
			timer = 0.f;
			CreateRandomObject();
		}
	}

	Engine::EndFrame();
	HandleNetMessages();
}

void GameServer::HandleNetMessages()
{
	myReportTimer += Crimson::Time::GetDeltaTime();
	if (myReportTimer >= 1.f)
	{
		myReportTimer = 0.f;
		myServer.ReportStatistics();
	}
	auto messages = myServer.Flush(Crimson::Time::GetDeltaTime());
	// This list now contains all incomming messages since Flush was last called
	// Currently all messages of worth are handled in callbacks, so flushing just to keep the server from filling up with unhandled messages
}

// TODO: Remove this function and includes, they are only here for a school assignment
#include "AssetManager/Assets/Components/Network/NetworkComponent.h"
#include "AssetManager/Assets/Components/Network/AssignmentComponent.h"
#include "AssetManager/Assets/Components/Collision/SphereColliderComponent.h"
void GameServer::CreateRandomObject()
{
	if (Engine::GetObjectManager().GetTemporaryObjects().size() < 10)
	{
		auto& object = *Engine::GetObjectManager().AddGameObject();
		object.SetPosition({ Crimson::Random::RandomNumber(-200.f, 200.f), 0.f, Crimson::Random::RandomNumber(-200.f, 200.f) });

		auto direction = Crimson::Vector3f::Null - object.GetWorldPosition();
		object.AddComponent(AssignmentComponent(direction));

		auto& mesh = object.AddComponent(AssetManager::GetAsset<MeshComponent>("sphere"));
		mesh.SetColor({ Crimson::Random::RandomNumber(1.f), Crimson::Random::RandomNumber(1.f) , Crimson::Random::RandomNumber(1.f) , 1.f });
		mesh.SetRenderShadow(false);

		object.AddComponent(SphereColliderComponent(50.f, Crimson::Vector3f::Null, false));

		auto& network = object.AddComponent<NetworkComponent>();
		network.SetSyncFrequency(Network::globalSyncFrequency);
		network.SyncTransform(true);

		SendCopyOfRandomObject(object);
	}
}

void GameServer::SendCopyOfRandomObject(const GameObject& anObject, Network::ClientInfo* aClient)
{
	GameObject copy(anObject.GetUUID());
	copy.SetPosition(anObject.GetWorldPosition());
	copy.AddComponent(anObject.GetComponent<MeshComponent>());

	SendCreateObjectMessage(copy, aClient);
}

void GameServer::HandleConnection(Network::ClientInfo& aClient)
{
	for (auto& [id, object] : Engine::GetObjectManager().GetTemporaryObjects())
	{
		SendCopyOfRandomObject(object, &aClient);
	}

	for (auto& [id, object] : myClientObjects)
	{
		SendCreateObjectMessage(object, &aClient);
	}
}

void GameServer::SendCreateObjectMessage(const GameObject& anObject, Network::ClientInfo* aClient)
{
	std::vector<uint8_t> data;
	{
		std::stringstream stream;
		anObject.Serialize(stream, true);
		data.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}

	if (data.size() <= Network::GetMaximumCreateGameobjectDataSize())
	{
		auto message = Network::CreateCreateGameObjectMessage(anObject.GetUUID(), data);
		if (aClient)
		{
			myServer.SendGuaranteedToClient(message, *aClient, false);
		}
		else
		{
			myServer.SendGuaranteedMessageToClients(message, nullptr,  false);
		}
	}
	else
	{
		constexpr unsigned short bufferSize = Network::GetMaximumCreateGameobjectDataSize();
		unsigned short totalPackets = static_cast<unsigned short>(data.size() / bufferSize);
		if (data.size() % bufferSize > 0u)
		{
			// Add one due to int rounding from previous division
			++totalPackets;
		}
		unsigned short id = myServer.GetMessageID();
		size_t dataLeft = data.size();
		uint8_t* pointer = data.data();
		for (unsigned short i = 0; i < totalPackets; i++)
		{
			std::vector<uint8_t> partialData;
			if (dataLeft < bufferSize)
			{
				partialData.resize(dataLeft);
				memcpy_s(partialData.data(), dataLeft, pointer, dataLeft);
			}
			else
			{
				partialData.resize(bufferSize);
				memcpy_s(partialData.data(), bufferSize, pointer, bufferSize);
				pointer = pointer + bufferSize;
				dataLeft -= bufferSize;
			}

			Network::NetMessage message = Network::CreateCreateGameObjectMessage(anObject.GetUUID(), partialData);
			message.needReply = true;
			message.messageID = id;
			message.packetIndex = i;
			message.totalPackets = totalPackets;
			if (aClient)
			{
				myServer.SendMultiMessageToClient(message, *aClient, false);
			}
			else
			{
				myServer.SendMultiMessageToClients(message, nullptr, false);
			}
		}
	}
}

void GameServer::SendDeleteObjectMessage(const UUIDv4::UUID& anId)
{
	auto message = Network::CreateDeleteGameObjectMessage(anId);
	myServer.SendGuaranteedMessageToClients(message, nullptr, false);
}

void GameServer::SendSetActiveMessage(bool aState, const UUIDv4::UUID& anID, const UUIDv4::UUID& aSenderID)
{
	constexpr rsize_t dataSize = sizeof(Network::GameObjectMessage::data);

	Network::GameObjectMessage message;
	message.id = anID;
	message.action = Network::ObjectAction::SetActive;
	message.size = sizeof(bool) + sizeof(double);

	// TODO: Rework timestamp to use a timepoint relative to servertime. 
	// Current idea: Server saves time when started and sends this timepoint to all clients that connects.
	// Should probably cache the current timepoint in Update
	double timestamp = Crimson::Time::GetTotalTime();
	constexpr int timestampOffset = sizeof(bool);
	memcpy_s(message.data, dataSize, &aState, sizeof(bool));
	memcpy_s(message.data + timestampOffset, dataSize - timestampOffset, &timestamp, sizeof(double));

	if (auto iter = myClientInfo.find(aSenderID); iter != myClientInfo.end())
	{
		myServer.SendGuaranteedToClient(Network::CreateGameObjectMessage(message), iter->second);
	}	
}

bool GameServer::HasAllCreateMessages(const UUIDv4::UUID& anId)
{
	std::sort(myMultipartCreateMessages.begin(), myMultipartCreateMessages.end(), Network::MultiMessageSort);

	unsigned short currentSender = 0;
	unsigned short currentMessage = 0;
	unsigned short previousIndex = 0;

	bool hasFoundUUID = false;
	for (auto iter = myMultipartCreateMessages.begin(); iter != myMultipartCreateMessages.end(); iter++)
	{
		const auto& message = *iter;
		if (Network::ExtractUUID(message) != anId)
		{
			if (hasFoundUUID)
			{
				return false;
			}
			else
			{
				continue;
			}
		}
		else if (!hasFoundUUID)
		{
			currentSender = message.senderID;
			currentMessage = message.messageID;
			previousIndex = message.packetIndex;
			hasFoundUUID = true;
			continue;
		}

		assert(currentSender == message.senderID && "Sender is not the same!");
		assert(currentMessage == message.messageID && "MessageID is not the same!");

		if (previousIndex != message.packetIndex - 1)
		{
			return false;
		}

		if (message.packetIndex == message.totalPackets - 1)
		{
			return true;
		}
		else
		{
			previousIndex = message.packetIndex;
		}
	}
	return false;
}

std::vector<Network::NetMessage*> GameServer::GetAllCreateMessages(const UUIDv4::UUID& anId)
{
	std::vector<Network::NetMessage*> objectMessages;
	Network::NetMessage* current = nullptr;
	unsigned short total = 0;

	for (auto& message : myMultipartCreateMessages)
	{
		if (Network::ExtractUUID(message) == anId)
		{
			current = &message;
			total = message.totalPackets;
			break;
		}
	}

	for (unsigned short i = 0; i < total; i++)
	{
		assert(anId == Network::ExtractUUID(*current) && "Not all messages belongs to the same UUID!");
		assert(current->packetIndex == i && "Messages are not arranged in order!");
		objectMessages.emplace_back(current);
		current++;
	}
	return objectMessages;
}

void GameServer::RemoveAllCreateMessages(const UUIDv4::UUID& anId)
{
	for (auto iter = myMultipartCreateMessages.begin(); iter != myMultipartCreateMessages.end();)
	{
		if (Network::ExtractUUID(*iter) == anId)
		{
			iter = myMultipartCreateMessages.erase(iter);
		}
		else 
		{
			iter++;
		}
	}
}

void GameServer::HandleCreateObject(Network::ClientInfo& client, const Network::NetMessage& aMessage)
{
	if (aMessage.totalPackets == 1u)
	{
		auto& id = Network::ExtractUUID(aMessage);
		myClientObjects.emplace(id, NetworkManager::ExtractCreatedGameObject(aMessage));
		myClientInfo.emplace(id, client);
	}
	else
	{
		myMultipartCreateMessages.emplace_back(aMessage);
		const auto& id = Network::ExtractUUID(aMessage);
		if (HasAllCreateMessages(id))
		{
			myClientObjects.emplace(id, NetworkManager::ExtractCreatedGameObject(GetAllCreateMessages(id)));
			myClientInfo.emplace(id, client);
			RemoveAllCreateMessages(id);
		}		
	}
}

void GameServer::HandleDeleteObject(Network::ClientInfo&, const Network::NetMessage& aMessage)
{
	auto& id = Network::ExtractUUID(aMessage);
	myClientObjects.erase(id);
	myClientInfo.erase(id);
}

void GameServer::HandleObjectMessage(Network::ClientInfo&, const Network::NetMessage& aMessage)
{
	if (auto iter = myClientObjects.find(Network::ExtractUUID(aMessage)); iter != myClientObjects.end())
	{
		iter->second.RecieveNetmessage(Network::ExtractGameObjectMessage(aMessage));
	}
}
