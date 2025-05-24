#pragma once
#include <string>
#include "EngineProtocol.h"

enum EEnginePacketType : __int8
{
	USER_ACCESS = -128, // Ŭ��� ������ �������� ����� �������� Ȯ���ϱ� ���ؼ� ���� ��ȯ�ϴ� ��Ŷ
};

class UUserAccessPacket : public UEngineProtocol
{
public:
	UUserAccessPacket()
	{
		SetPacketType(EEnginePacketType::USER_ACCESS);
	}
};
