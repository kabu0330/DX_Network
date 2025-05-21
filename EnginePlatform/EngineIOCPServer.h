#pragma once
#include <MSWSock.h>

#include "EngineNetwork.h"
#include "EngineNetworkData.h"

// Ό³Έν :
class UEngineIOCPServer : public UEngineNetwork
{
public:
	UEngineIOCPServer();
	~UEngineIOCPServer();

	ENGINEAPI void OpenIOCPServer(int _Port);
	void Release() override;

protected:
	void CreateIOCPSockets();
	void OpenListenServer();
	void CreateIOCP();
	void GetAcceptEx();
	void SetWorkerThread();

	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	
	HANDLE AcceptPort = nullptr;
	HANDLE SendRecvPort = nullptr;

	LPFN_ACCEPTEX FnAccetEx = nullptr;

	CRITICAL_SECTION Lock;

	std::unordered_map<SOCKET, FClientData> AllClients;

	std::atomic<int> SessionTokenCreator;
	std::atomic<int> ObjectTokenCreator;

private:
	// delete Function
	UEngineIOCPServer(const UEngineIOCPServer& _Other) = delete;
	UEngineIOCPServer(UEngineIOCPServer&& _Other) noexcept = delete;
	UEngineIOCPServer& operator=(const UEngineIOCPServer& _Other) = delete;
	UEngineIOCPServer& operator=(UEngineIOCPServer&& _Other) noexcept = delete;
};

