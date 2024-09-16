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

private:
	Network::Server myServer;

	void HandleCrash(const std::exception& anException);

	void Init();
	void Update();

	void CreateRandomObject();

	void HandleConnection(Network::ClientInfo& aClient);
	void SendCreateObjectMessage(const GameObject& anObject, Network::ClientInfo& aClient);
	void SendDeleteObjectMessage(const UUIDv4::UUID& anId);
};

