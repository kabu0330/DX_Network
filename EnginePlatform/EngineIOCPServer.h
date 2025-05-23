#pragma once
#include <MSWSock.h>

#include "EngineNetwork.h"
#include "EngineNetworkData.h"
#include "EngineServer.h"

// Ό³Έν :
class UEngineIOCPServer : public UEngineServer
{
public:
	ENGINEAPI UEngineIOCPServer();
	ENGINEAPI ~UEngineIOCPServer();

	ENGINEAPI void OpenIOCPServer(int _Port);
	void Release() override;

protected:
	void CreateListenSocket();
	void CreateAcceptSocket();
	void OpenListenServer();
	void CreateIOCP();
	void GetAcceptEx();
	void SetWorkThread();
	
	void CallAcceptEx();
	void AcceptThread(UEngineIOCPServer* _Server);
	bool SetWSARecv(char* _Buffer, FOverlappedEx* _Context);

	//std::shared_ptr<char[]> CreateBuffer();

	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	
	HANDLE AcceptPort = nullptr;
	HANDLE SendRecvPort = nullptr;

	LPFN_ACCEPTEX FnAccetEx = nullptr;
	OVERLAPPED AcceptOverlapped = {};

	CRITICAL_SECTION Lock;

	std::unordered_map<SOCKET, FClientData> AllClients;



private:
	// delete Function
	UEngineIOCPServer(const UEngineIOCPServer& _Other) = delete;
	UEngineIOCPServer(UEngineIOCPServer&& _Other) noexcept = delete;
	UEngineIOCPServer& operator=(const UEngineIOCPServer& _Other) = delete;
	UEngineIOCPServer& operator=(UEngineIOCPServer&& _Other) noexcept = delete;
};

