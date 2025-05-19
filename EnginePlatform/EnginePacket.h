#pragma once
#include "EngineProtocol.h"

enum EEnginePacketType : __int8
{
	UserAccessPacket = -100, // Ŭ��� ������ �������� ����� �������� Ȯ���ϱ� ���ؼ� ���� ��ȯ�ϴ� ��Ŷ
};

class UUserAccessPacket : public UEngineProtocol
{
public:
	UUserAccessPacket()
	{
		SetPacketType(EEnginePacketType::UserAccessPacket);
	}
};
