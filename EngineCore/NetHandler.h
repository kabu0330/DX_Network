#pragma once
#include <iostream>
#include "Level.h"
#include "GameMode.h"
#include <EnginePlatform/EngineProtocol.h>
#include "NetObject.h"
#include <Engineplatform/EngineNetwork.h>
#include <EnginePlatform/EngineDispatcher.h>

// 설명 : 네트워크 기반 함수 래핑 클래스
// 프로토콜 정의 -> 처리 방식 결정 -> 전송 -> 처리 실행 순에서
// 처리 방식을 결정하고 전송하는 역할을 간결한 인터페이스로 제공
class UNetHandler
{
public:
	UNetHandler(UNetObject* _NetObject, AActor* _Actor);
	~UNetHandler();

	template<typename Packet>
	void SendPacket(std::function<void(std::shared_ptr<Packet>)> _SyncFunction, bool bIsOneShot = false)
	{
		if (nullptr == Actor && nullptr == NetObject)
		{
			return;
		}
		
		static_assert(std::is_base_of_v<UEngineProtocol, Packet>, "패킷 타입은 UEngineProtocol을 상속해야 합니다");

		AGameMode* GameMode = Actor->GetWorld()->GetGameMode();
		if (nullptr == GameMode->GetNetwork())
		{
			return;
		}

		// Tick에서 매번 SendPacket을 보내는 패킷과 최초 한 번만 보내는 패킷을 구분한다.
		// 최초 1회 패킷을 보내는 경우 이 구간을 생략
		static float FrameAccumulator = 0.0f;
		constexpr float FrameInterval = 1.0f / 60.0f;

		if (false == bIsOneShot)
		{
			float DeltaTime = UEngineCore::GetDeltaTime();
			FrameAccumulator += DeltaTime;
			if (FrameAccumulator < FrameInterval)
			{
				return;
			}

			FrameAccumulator = 0.0f;
		}

		std::shared_ptr<Packet> NewPacket = NetObject->CreatePacket<Packet>();
		if (nullptr == NewPacket)
		{
			MSGASSERT("패킷 생성 실패");
			return;
		}

		_SyncFunction(NewPacket);

		GameMode->GetNetwork()->SendPacket(NewPacket);
	}

	// 이렇게 짜는게 합당한지 좀 더 고민해봐야...
	// 호출 방식이 NetObject를 상속받은 액터만이 호출할 수 있는데 패킷을 처리하는 방식을 항상 네트워크 오브젝트를 통해서 
	// 함수를 등록하는 방식이 옳은지?
	
	//template<typename Packet, typename PacketType>
	//void ProcPacket(PacketType _Type, std::function<void(std::shared_ptr<Packet>)> _SyncFunction)
	//{
	//	static_assert(std::is_base_of_v<UEngineProtocol, Packet>, "패킷 타입은 UEngineProtocol을 상속해야 합니다");

	//	AGameMode* GameMode = Actor->GetWorld()->GetGameMode();
	//	if (nullptr == GameMode)
	//	{
	//		return;
	//	}
	//	UEngineNetwork* Network = GameMode->GetNetwork().get();

	//	Network->GetDispatcher().AddHandler<Packet>(static_cast<int>(_Type), [_SyncFunction, Network](std::shared_ptr<Packet> _Packet)
	//	{
	//		_SyncFunction;

	//		UEngineServer* Server = dynamic_cast<UEngineServer*>(Network);
	//		if (nullptr != Server)
	//		{
	//			Network->SendPacket(_Packet); // 다른 클라에게도 전송
	//		}
	//	});
	//}

protected:

private:
	UNetObject* NetObject = nullptr;
	AActor* Actor = nullptr;

private:

	// delete Function
	UNetHandler(const UNetHandler& _Other) = delete;
	UNetHandler(UNetHandler&& _Other) noexcept = delete;
	UNetHandler& operator=(const UNetHandler& _Other) = delete;
	UNetHandler& operator=(UNetHandler&& _Other) noexcept = delete;
};

