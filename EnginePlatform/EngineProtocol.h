#pragma once
#include <memory>

#include <EngineBase/EngineSerializer.h>
#include <EngineBase/EngineMath.h>

enum EEnginePacketType
{
	UserAccessPacket = -2, // 클라와 서버가 정상적인 통신이 가능한지 확인하기 위해서 서로 교환하는 패킷
};

// 설명 :
class EngineProtocol : public std::enable_shared_from_this<EngineProtocol>, public ISerializeObject
{
public:
	EngineProtocol();
	~EngineProtocol();



protected:

private:
	// delete Function
	EngineProtocol(const EngineProtocol& _Other) = delete;
	EngineProtocol(EngineProtocol&& _Other) noexcept = delete;
	EngineProtocol& operator=(const EngineProtocol& _Other) = delete;
	EngineProtocol& operator=(EngineProtocol&& _Other) noexcept = delete;
};

