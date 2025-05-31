#pragma once
#include <iostream>
#include "Level.h"
#include "GameMode.h"
#include <EnginePlatform/EngineProtocol.h>
#include "NetObject.h"
#include <Engineplatform/EngineNetwork.h>

// 설명 :
class UNetPacketSender
{
public:
	~UNetPacketSender() = default;

	template<typename PacketType>
	static void SendPacket(UNetObject* _NetObject, ULevel* _World, std::function<void(std::shared_ptr<PacketType>)> _SyncFunction)
	{
		if (nullptr == _World)
		{
			return;
		}

		static_assert(std::is_base_of_v<UEngineProtocol, PacketType>, "패킷 타입은 UEngineProtocol을 상속해야 합니다");

		AGameMode* GameMode = _World->GetGameMode();
		if (nullptr == GameMode->GetNetwork())
		{
			std::cout << "싱글 모드에서 SendPacket() 함수를 호출했습니다." << std::endl;
			return;
		}

		static float FrameAccumulator = 0.0f;
		constexpr float FrameInterval = 1.0f / 60.0f;

		float DeltaTime = UEngineCore::GetDeltaTime();
		FrameAccumulator += DeltaTime;
		if (FrameAccumulator < FrameInterval)
		{
			return;
		}

		FrameAccumulator = 0.0f;

		std::shared_ptr<PacketType> Packet = _NetObject->CreatePacket<PacketType>();
		if (nullptr == Packet)
		{
			MSGASSERT("패킷 생성 실패");
			return;
		}

		_SyncFunction(Packet);

		GameMode->GetNetwork()->SendPacket(Packet);
	}

protected:

private:

private:
	UNetPacketSender() {};

	// delete Function
	UNetPacketSender(const UNetPacketSender& _Other) = delete;
	UNetPacketSender(UNetPacketSender&& _Other) noexcept = delete;
	UNetPacketSender& operator=(const UNetPacketSender& _Other) = delete;
	UNetPacketSender& operator=(UNetPacketSender&& _Other) noexcept = delete;
};

