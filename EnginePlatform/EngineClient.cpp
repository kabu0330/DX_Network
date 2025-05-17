#include "PreCompile.h"
#include "EngineClient.h"

UEngineClient::UEngineClient()
{
}

UEngineClient::~UEngineClient()
{
}

void UEngineClient::Connect(std::string_view _IP, int _Port)
{
	StartNetwork();

	IPAddress = _IP;
	Port = _Port;

	ConnectSocket = CreateSocket(ENetworkType::TCP);
	SOCKADDR_IN Address = CreateAddress(_IP.data(), _Port);

	if (SOCKET_ERROR == connect(ConnectSocket, (const sockaddr*)&Address, sizeof(SOCKADDR_IN)))
	{
		ENGINEMSG("클라이언트 소켓 생성에 실패했습니다.");
		return;
	}

	RecvThread = std::make_shared<UEngineThread>();
	RecvThread->Start("Client Recv Thread", std::bind(&UEngineNetwork::RecvTCPThreadFunction, this, ConnectSocket));
}

void UEngineClient::SendPacket(UEngineProtocol* _Protocol)
{
	if (-1 == ConnectSocket)
	{
		return;
	}

	UEngineSerializer Ser;
	_Protocol->SerializePacket(Ser);

	send(ConnectSocket, Ser.GetDataBuffer(), Ser.GetWritePos(), 0);
}

