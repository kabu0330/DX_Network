#include "PreCompile.h"
#include "EngineNetwork.h"

#include <EngineBase/EngineSerializer.h>


bool bIsNetworking = false;

UEngineNetwork::UEngineNetwork()
{
}

UEngineNetwork::~UEngineNetwork()
{
    Release();
}

void UEngineNetwork::Release()
{
    bIsActive = false;
    Dispatcher.Release();
    ProtocolFunction = nullptr;
    DisConnectFunction = nullptr;

    if (true == bIsNetworking)
    {
        WSACleanup();
        bIsNetworking = false;
    }
}

void UEngineNetwork::StartNetwork()
{
    if (true == bIsNetworking)
    {
        return;
    }

    WSAData WsaData;
    int Error = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == Error)
    {
        MSGASSERT("OS 네트워크 설정에 실패했습니다.");
        return;
    }

    bIsNetworking = true;
}

SOCKET UEngineNetwork::CreateSocket(ENetworkType _Type)
{
    SOCKET Result = -1;
    switch (_Type)
    {
    case TCP:
        Result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        break;
    case UDP:
        Result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        break;
    default:
        MSGASSERT("Engine에서 지원하지 않는 네트워크 통신 타입입니다.");
        return -1;
    }

    if (INVALID_SOCKET == Result)
    {
        MSGASSERT("소켓 생성에 실패했습니다.");
        return -1;
    }

    return Result;
}

SOCKADDR_IN UEngineNetwork::CreateAddress(std::string _IP, int _Port)
{
    SOCKADDR_IN Address = {};
    Address.sin_family = AF_INET;
    Address.sin_port = htons(_Port);
    if (SOCKET_ERROR == inet_pton(AF_INET, _IP.c_str(), &Address.sin_addr))
    {
        MSGASSERT("inet_pton 주소 바이트 정렬에 실패했습니다.");
        return SOCKADDR_IN();
    }

    return Address;
}

bool UEngineNetwork::Bind(SOCKET _Socket, const SOCKADDR_IN& _Address)
{
    if (INVALID_SOCKET == bind(_Socket, (const sockaddr*)&_Address, sizeof(SOCKADDR_IN)))
    {
        MSGASSERT("소켓 바인드 실패");
        return false;
    }

    return true;
}

void UEngineNetwork::RecvTCPThreadFunction(UEngineNetwork* _Server, SOCKET _Socket)
{
    UEngineSerializer Ser;
    Ser.DataResize(1024);

    // 1. 패킷 수신 루프
    while (true == _Server->bIsActive)
    {
        int PacketSize = recv(_Socket, Ser.GetWritePointer(), static_cast<int>(Ser.GetWritableBites()), 0);
        
        if (-1 == PacketSize)
        {
            if (nullptr != _Server->DisConnectFunction)
            {
                _Server->DisConnectFunction();
            }
            return;
        }

        Ser.AddWritePos(PacketSize);
        int CurPacketSize = Ser.GetWritePos();
        
        // 엔진 프로토콜 크기보다 작다면 패킷이 다 도착하지 않은 것
        if (UEngineProtocol::GetProtocolSize() > CurPacketSize)
        {
            continue;
        }

        int TotalPacketSize = Ser.ReadDataAt<int>(4); 

        // 패킷이 다 도착할 때까지 기다린다.
        if (TotalPacketSize > CurPacketSize)
        {
            continue; // 패킷이 DataResize(1024)보다 크면 확장을 고려해야 한다. 
        }

        if (false == _Server->bIsActive)
        {
            return;
        }

        // 2. 패킷 처리 루프
        while (true == _Server->bIsActive)
        {
            int PacketType = Ser.ReadDataAt<int>(); // 엔진 프로토콜의 첫 4byte는 패킷 타입
            std::shared_ptr<UEngineProtocol> Packet = _Server->Dispatcher.ConvertPacket(PacketType, Ser);

            if (false == _Server->bIsActive)
            {
                return;
            }

            if (nullptr != Packet) // 패킷 처리
            {
                _Server->ProtocolFunction(Packet);
            }

            // 다음 패킷 처리
            Ser.CompactBuffer();

            int NextPacketSize = Ser.ReadDataAt<int>(4);
            if (NextPacketSize > Ser.GetWritePos())
            {
                break;
            }
        }
    }

}



