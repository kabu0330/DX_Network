#pragma once
#include <map>
#include <functional>
#include <memory>

#include <EngineBase/EngineDebug.h>
#include "EngineProtocol.h"

// ���� : ��Ŷ�� ó���ϴ� �Լ��� �����ϰ� �����ϴ� Ŭ����
class UEngineDispatcher
{
public:
	UEngineDispatcher();
	~UEngineDispatcher();

	template<typename PacketType>
	void AddHandler(int _Type, std::function<void(std::shared_ptr<PacketType>)> _Callback)
	{
		if (true == PacketHandler.contains(_Type))
		{
			MSGASSERT("�̹� ��ϵ� �ڵ��Դϴ�.");
			return;
		}

		ConvertPacketHandler[_Type] = [](UEngineSerializer& _Ser)
			{
				std::shared_ptr<UEngineProtocol> NewProtocal = std::make_shared<PacketType>();
				NewProtocal->DeSerialize(_Ser);
				return NewProtocal;
			};

		PacketHandler[_Type] = [_Callback](std::shared_ptr<UEngineProtocol> _Packet)
			{
				std::shared_ptr<PacketType> UserPacket = std::dynamic_pointer_cast<PacketType>(_Packet);
				if (nullptr == UserPacket)
				{
					MSGASSERT("�������� �ʴ� ��Ŷ Ÿ���Դϴ�.");
					return;
				}

				_Callback(UserPacket);
			};
	}

	std::shared_ptr<UEngineProtocol> ConvertPacket(int _Type, UEngineSerializer& _Ser)
	{
		if (false == ConvertPacketHandler.contains(_Type))
		{
			MSGASSERT("����Ʈ �Լ��� �������� �ʽ��ϴ�.\n AddHandler()�� ����Ʈ ��Ŷ �ڵ��� �߰��� �ּ���.");
			return nullptr;
		}

		return ConvertPacketHandler[_Type](_Ser);
	}

	void ProcessPacket(std::shared_ptr<UEngineProtocol> _Packet)
	{
		if (false == PacketHandler.contains(_Packet->GetPacketType()))
		{
			MSGASSERT("�ش� ��Ŷ�� �ڵ��� �����ϴ�.\n AddHandler()�� ��Ŷ �ڵ鷯�� �߰��� �ּ���.");
			return;
		}

		PacketHandler[_Packet->GetPacketType()](_Packet);
	}

	void Release();

protected:

private:
	std::map<int, std::function<std::shared_ptr<UEngineProtocol>(UEngineSerializer& _Ser)>> ConvertPacketHandler;
	std::map<int, std::function<void(std::shared_ptr<UEngineProtocol>)>> PacketHandler;


private:
	// delete Function
	UEngineDispatcher(const UEngineDispatcher& _Other) = delete;
	UEngineDispatcher(UEngineDispatcher&& _Other) noexcept = delete;
	UEngineDispatcher& operator=(const UEngineDispatcher& _Other) = delete;
	UEngineDispatcher& operator=(UEngineDispatcher&& _Other) noexcept = delete;
};

