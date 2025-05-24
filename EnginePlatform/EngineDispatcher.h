#pragma once
#include <map>
#include <functional>
#include <memory>

#include <EngineBase/EngineDebug.h>
#include "EngineProtocol.h"

// 설명 : 패킷을 처리하는 함수를 저장하고 실행하는 클래스
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
			MSGASSERT("이미 등록된 핸들입니다.");
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
					MSGASSERT("존재하지 않는 패킷 타입입니다.");
					return;
				}

				_Callback(UserPacket);
			};
	}

	std::shared_ptr<UEngineProtocol> ConvertPacket(int _Type, UEngineSerializer& _Ser)
	{
		if (false == ConvertPacketHandler.contains(_Type))
		{
			MSGASSERT("컨버트 함수가 존재하지 않습니다.\n AddHandler()로 컨버트 패킷 핸들을 추가해 주세요.");
			return nullptr;
		}

		return ConvertPacketHandler[_Type](_Ser);
	}

	void ProcessPacket(std::shared_ptr<UEngineProtocol> _Packet)
	{
		if (false == PacketHandler.contains(_Packet->GetPacketType()))
		{
			MSGASSERT("해당 패킷의 핸들이 없습니다.\n AddHandler()로 패킷 핸들러를 추가해 주세요.");
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

