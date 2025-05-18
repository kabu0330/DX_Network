#pragma once
#include "EngineNetwork.h"
#include "EngineThread.h"

// Ό³Έν :
class UEngineClient : public UEngineNetwork
{
public:
	ENGINEAPI UEngineClient();
	ENGINEAPI ~UEngineClient();

	ENGINEAPI void Connect(std::string_view _IP, int _Port);
	
	ENGINEAPI void SendPacket(UEngineProtocol* _Protocol) override;

protected:

private:
	SOCKET ConnectSocket = -1;
	std::shared_ptr<UEngineThread> RecvThread;


private:
	// delete Function
	UEngineClient(const UEngineClient& _Other) = delete;
	UEngineClient(UEngineClient&& _Other) noexcept = delete;
	UEngineClient& operator=(const UEngineClient& _Other) = delete;
	UEngineClient& operator=(UEngineClient&& _Other) noexcept = delete;
};

