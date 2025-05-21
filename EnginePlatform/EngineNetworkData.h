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

class UPlatformData
{
public:
	static const int BackLog = SOMAXCONN;
};

