#pragma once
#include "NetworkServer/Server.h"
#include "AssetManager/Assets/GameObject.h"
#include "GameplayEngine/PostMaster/Observer.h"

class GameServer: public Observer
{
public:
	GameServer() = default;
	~GameServer() = default;

	bool Initialize();
	int Run();
	void Shutdown();

	void RecieveMessage(const Crimson::Message& aMessage) override;

	void SendTransformChanged(const Transform& aTransform, float aTimeUntilNextSync, const UUIDv4::UUID& anID, const UUIDv4::UUID& aRecipientID = GameObject::nullUUID);

	const std::unordered_map<UUIDv4::UUID, GameObject>& GetClientObjects() const;

private:
	Network::Server myServer;
	std::unordered_map<UUIDv4::UUID, GameObject> myClientObjects;
	std::unordered_map<UUIDv4::UUID, Network::ClientInfo> myClientInfo;
	std::vector<UUIDv4::UUID> myDeletedClients;
	std::vector<Network::NetMessage> myMultipartCreateMessages;
	float myReportTimer = 0.f;

	void HandleCrash(const std::exception& anException);

	void Init();
	void Update();

	void HandleNetMessages();

	void CreateRandomObject();
	void SendCopyOfRandomObject(const GameObject& anObject, Network::ClientInfo* aClient = nullptr);

	void HandleConnection(Network::ClientInfo& aClient);
	void SendCreateObjectMessage(const GameObject& anObject, Network::ClientInfo* aClient = nullptr);
	void SendDeleteObjectMessage(const UUIDv4::UUID& anId);
	void SendSetActiveMessage(bool aState, const UUIDv4::UUID& anID, const UUIDv4::UUID& aSenderID);

	bool HasAllCreateMessages(const UUIDv4::UUID& anId);
	std::vector<Network::NetMessage*> GetAllCreateMessages(const UUIDv4::UUID& anId);
	void RemoveAllCreateMessages(const UUIDv4::UUID& anId);

	void HandleCreateObject(Network::ClientInfo& aClient, const Network::NetMessage& aMessage);
	void HandleDeleteObject(Network::ClientInfo& aClient, const Network::NetMessage& aMessage);
	void HandleObjectMessage(Network::ClientInfo& aClient, const Network::NetMessage& aMessage);
};

