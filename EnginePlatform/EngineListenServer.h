#pragma once
#include "mutex"
#include <unordered_map>

#include "EngineNetwork.h"
#include "EngineThread.h"
#include "EngineServer.h"

// Ό³Έν :
class UEngineListenServer : public UEngineServer
{
public:
	ENGINEAPI UEngineListenServer();
	ENGINEAPI ~UEngineListenServer();

	ENGINEAPI void OpenServer(int _Port, int _BackLog = 512);

	ENGINEAPI static void AcceptThreadFunction(UEngineListenServer* _Server, SOCKET _ListenSocket);

	void SendPacket(std::shared_ptr<UEngineProtocol> _Protocol)
	{
		SendPacket(_Protocol.get());
	}
	ENGINEAPI void SendPacket(UEngineProtocol* _Protocol) override;

	ENGINEAPI void Release() override;


protected:

private:
	int BackLog = -1;
	SOCKET ListenSocket = -1;
	UEngineThread ConnectAcceptThread;
	
	std::mutex UserLock;

	std::unordered_map<int, SOCKET> AllUserSockets;
	std::vector<std::shared_ptr<UEngineThread>> AllUserThreads;

	std::atomic<bool> bAcceptInflight = false;


private:
	// delete Function
	UEngineListenServer(const UEngineListenServer& _Other) = delete;
	UEngineListenServer(UEngineListenServer&& _Other) noexcept = delete;
	UEngineListenServer& operator=(const UEngineListenServer& _Other) = delete;
	UEngineListenServer& operator=(UEngineListenServer&& _Other) noexcept = delete;
};

