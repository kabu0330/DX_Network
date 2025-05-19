#pragma once
#include "EngineNetwork.h"
#include "EngineThread.h"
#include "mutex"
#include <unordered_map>

// Ό³Έν :
class UEngineServer : public UEngineNetwork
{
public:
	ENGINEAPI UEngineServer();
	ENGINEAPI ~UEngineServer();

	ENGINEAPI void OpenServer(int _Port, int _BackLog = 512);
	ENGINEAPI void OpenIOCPServer(int _Port, int _BackLog = 512);

	ENGINEAPI static void AcceptThreadFunction(UEngineServer* _Server, SOCKET _ListenSocket);

	void SendPacket(std::shared_ptr<UEngineProtocol> _Protocol)
	{
		SendPacket(_Protocol.get());
	}
	ENGINEAPI void SendPacket(UEngineProtocol* _Protocol) override;

	ENGINEAPI int CreateObjectToken();
	ENGINEAPI int CreateSessionToken();

	ENGINEAPI void Release() override;


protected:

private:
	int BackLog = -1;
	SOCKET ListenSocket = -1;
	UEngineThread ConnectAcceptThread;
	
	std::mutex UserLock;

	std::unordered_map<int, SOCKET> AllUserSockets;
	std::vector<std::shared_ptr<UEngineThread>> AllUserThreads;

	std::atomic<int> SessionTokenCreator;
	std::atomic<int> ObjectTokenCreator;

	std::atomic<bool> bAcceptInflight = false;



private:
	// delete Function
	UEngineServer(const UEngineServer& _Other) = delete;
	UEngineServer(UEngineServer&& _Other) noexcept = delete;
	UEngineServer& operator=(const UEngineServer& _Other) = delete;
	UEngineServer& operator=(UEngineServer&& _Other) noexcept = delete;
};

