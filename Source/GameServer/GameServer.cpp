#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <fstream>

#include "GameServer.h"

#include "NetworkShared/MessageFunctions.h"

#include "GameplayEngine/Engine.h"
#include "GameplayEngine/PostMaster/PostMaster.h"
#include "GameplayEngine/Managers/SceneManager.h"
#include "GameplayEngine/Managers/ObjectManager.h"

#include "AssetManager/AssetManager.h"

#include "CrimsonUtilities/Time/Time.h"
#include "CrimsonUtilities/Math/Random.h"

bool GameServer::Initialize()
{
#ifndef _DEBUG
	try
	{
#endif // _DEBUG
		Engine::Init(NULL, Crimson::Vector2i::Null, false, true);

		myServer.Init(&Engine::GetLogger());
		myServer.SetConnectionCallback([this](Network::ClientInfo& aClient) { HandleConnection(aClient); });

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
	default:
		break;
	}
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
	myServer.Update();
}

#include "AssetManager/Assets/Components/Network/NetworkComponent.h"
#include "AssetManager/Assets/Components/Network/AssignmentComponent.h"
#include "AssetManager/Assets/Components/Collision/SphereColliderComponent.h"
void GameServer::CreateRandomObject()
{
	if (Engine::GetObjectManager().GetTemporaryObjects().size() < 10)
	{
		auto& object = *Engine::GetObjectManager().AddGameObject();
		object.SetPosition({Crimson::Random::RandomNumber(-200.f, 200.f), 0.f, Crimson::Random::RandomNumber(-200.f, 200.f) });

		auto direction = Crimson::Vector3f::Null - object.GetWorldPosition();
		object.AddComponent(AssignmentComponent(direction));

		auto& mesh = object.AddComponent(AssetManager::GetAsset<MeshComponent>("sphere"));
		mesh.SetColor({ Crimson::Random::RandomNumber(1.f), Crimson::Random::RandomNumber(1.f) , Crimson::Random::RandomNumber(1.f) , 1.f });
		mesh.SetRenderShadow(false);

		object.AddComponent(SphereColliderComponent(50.f, Crimson::Vector3f::Null, false));

		auto& network = object.AddComponent<NetworkComponent>();
		network.SetSyncFrequency(Network::globalSyncFrequency);
	}	
}

void GameServer::HandleConnection(Network::ClientInfo& aClient)
{
	for (auto& [id, object] : Engine::GetObjectManager().GetTemporaryObjects())
	{
		GameObject copy(object.GetUUID());
		copy.SetPosition(object.GetWorldPosition());
		copy.AddComponent(object.GetComponent<MeshComponent>());

		SendCreateObjectMessage(copy, aClient);
	}
}

void GameServer::SendCreateObjectMessage(const GameObject& anObject, Network::ClientInfo& aClient)
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
		message.messageID = myServer.GetMessageID();
		myServer.SendToClient(message, aClient);
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
			message.messageID = id;
			message.packetIndex = i;
			message.totalPackets = totalPackets;
			myServer.SendToClient(message, aClient);
		}
	}
}

void GameServer::SendDeleteObjectMessage(const UUIDv4::UUID& anId)
{
	myServer.SendMessageToClients(Network::CreateDeleteGameObjectMessage(anId));
}
