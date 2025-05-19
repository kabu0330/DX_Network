#pragma once
#include "EngineProtocol.h"
#include "EngineDispatcher.h"

enum ENetworkType
{
	TCP,
	UDP,
	IOCP
};

// 설명 :
class UEngineNetwork
{
public:
	UEngineNetwork();
	virtual ~UEngineNetwork();

	// Windows 네트워크 설정 함수
	ENGINEAPI static void StartNetwork();
	ENGINEAPI SOCKET	  CreateSocket	(ENetworkType _Type);
	ENGINEAPI SOCKADDR_IN CreateAddress (std::string _IP, int _Port);
	ENGINEAPI bool		  Bind			(SOCKET _Socket, const SOCKADDR_IN& _Address);
	//

	ENGINEAPI void SendPacket(std::shared_ptr<UEngineProtocol> _Protocol)
	{
		SendPacket(_Protocol.get());
	}
	ENGINEAPI virtual void SendPacket(UEngineProtocol* _Protocol) {};


	static void RecvTCPThreadFunction(UEngineNetwork* _Server, SOCKET _Socket);

	bool IsNetworking()
	{
		return -1 != SessionToken;
	}

	void SetUserAccessFunction(std::function<void(std::shared_ptr<UUserAccessPacket> _Packet)> _UserAccessFunction)
	{
		Dispatcher.AddHandler(static_cast<int>(EEnginePacketType::UserAccessPacket), _UserAccessFunction);
	}
	void SetProtocolFunction(std::function<void(std::shared_ptr<UEngineProtocol> _Packet)> _ProtocolFunction)
	{
		ProtocolFunction = _ProtocolFunction;
	}

	int GetSessionToken() const
	{
		return SessionToken;
	}

	UEngineDispatcher& GetDispatcher() 
	{
		return Dispatcher;
	}

	virtual void Release();


protected:
	int SessionToken = -1;
	std::string IPAddress = "";
	int Port = -1;

	bool bIsActive = true;

	UEngineDispatcher Dispatcher;

	std::function<void()> DisConnectFunction = nullptr;
	std::function<void(std::shared_ptr<UEngineProtocol> _Packet)> ProtocolFunction = nullptr;

private:
	// delete Function
	UEngineNetwork(const UEngineNetwork& _Other) = delete;
	UEngineNetwork(UEngineNetwork&& _Other) noexcept = delete;
	UEngineNetwork& operator=(const UEngineNetwork& _Other) = delete;
	UEngineNetwork& operator=(UEngineNetwork&& _Other) noexcept = delete;
};

