#pragma once
#include <string>
#include "EngineProtocol.h"

enum EEnginePacketType : __int8
{
	USER_ACCESS = -128, // 클라와 서버가 정상적인 통신이 가능한지 확인하기 위해서 서로 교환하는 패킷
};

class UUserAccessPacket : public UEngineProtocol
{
public:
	UUserAccessPacket()
	{
		SetPacketType(EEnginePacketType::USER_ACCESS);
	}
};
