#pragma once
#include <memory>

#include <EngineBase/EngineSerializer.h>
#include <EngineBase/EngineMath.h>

enum EEnginePacketType
{
	UserAccessPacket = -2, // Ŭ��� ������ �������� ����� �������� Ȯ���ϱ� ���ؼ� ���� ��ȯ�ϴ� ��Ŷ
};

// ���� :
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

