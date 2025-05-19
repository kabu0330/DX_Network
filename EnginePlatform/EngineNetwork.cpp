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
        MSGASSERT("OS ��Ʈ��ũ ������ �����߽��ϴ�.");
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
        MSGASSERT("Engine���� �������� �ʴ� ��Ʈ��ũ ��� Ÿ���Դϴ�.");
        return -1;
    }

    if (INVALID_SOCKET == Result)
    {
        MSGASSERT("���� ������ �����߽��ϴ�.");
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
        MSGASSERT("inet_pton �ּ� ����Ʈ ���Ŀ� �����߽��ϴ�.");
        return SOCKADDR_IN();
    }

    return Address;
}

bool UEngineNetwork::Bind(SOCKET _Socket, const SOCKADDR_IN& _Address)
{
    if (INVALID_SOCKET == bind(_Socket, (const sockaddr*)&_Address, sizeof(SOCKADDR_IN)))
    {
        MSGASSERT("���� ���ε� ����");
        return false;
    }

    return true;
}

void UEngineNetwork::RecvTCPThreadFunction(UEngineNetwork* _Server, SOCKET _Socket)
{
    UEngineSerializer Ser;
    Ser.DataResize(1024);

    // 1. ��Ŷ ���� ����
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
        
        // ���� �������� ũ�⺸�� �۴ٸ� ��Ŷ�� �� �������� ���� ��
        if (UEngineProtocol::GetProtocolSize() > CurPacketSize)
        {
            continue;
        }

        int TotalPacketSize = Ser.ReadDataAt<int>(4); 

        // ��Ŷ�� �� ������ ������ ��ٸ���.
        if (TotalPacketSize > CurPacketSize)
        {
            continue; // ��Ŷ�� DataResize(1024)���� ũ�� Ȯ���� ����ؾ� �Ѵ�. 
        }

        if (false == _Server->bIsActive)
        {
            return;
        }

        // 2. ��Ŷ ó�� ����
        while (true == _Server->bIsActive)
        {
            int PacketType = Ser.ReadDataAt<int>(); // ���� ���������� ù 4byte�� ��Ŷ Ÿ��
            std::shared_ptr<UEngineProtocol> Packet = _Server->Dispatcher.ConvertPacket(PacketType, Ser);

            if (false == _Server->bIsActive)
            {
                return;
            }

            if (nullptr != Packet) // ��Ŷ ó��
            {
                _Server->ProtocolFunction(Packet);
            }

            // ���� ��Ŷ ó��
            Ser.CompactBuffer();

            int NextPacketSize = Ser.ReadDataAt<int>(4);
            if (NextPacketSize > Ser.GetWritePos())
            {
                break;
            }
        }
    }

}



