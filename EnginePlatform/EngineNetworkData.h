#pragma once

enum EOperationType
{
	ACCEPT,
	READ,
	WRITE,
};

struct FClientData
{
	std::string Name = "";
};

struct FOverlappedEx
{
	OVERLAPPED Overlapped;
	WSABUF WsaBuf;
	char* Buffer;
	SOCKET Socket;
	EOperationType OperationType;

	FOverlappedEx()
	{
		ZeroMemory(this, sizeof(FOverlappedEx));
	}
	void ClearOverlapped()
	{
		ZeroMemory(&Overlapped, sizeof(OVERLAPPED));
	}
};

class UNetData
{
public:
	static const int BackLog = SOMAXCONN;
	static const int SockAddrSize = (sizeof(sockaddr_in) + 16);
	inline static char AcceptBuffer[SockAddrSize * 2] = {};
	static const int BufferSize = 1024;
};

