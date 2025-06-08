#pragma once
#include <iostream>
#include "Level.h"
#include "GameMode.h"
#include <EnginePlatform/EngineProtocol.h>
#include "NetObject.h"
#include <Engineplatform/EngineNetwork.h>
#include <EnginePlatform/EngineDispatcher.h>

// ���� : ��Ʈ��ũ ��� �Լ� ���� Ŭ����
// �������� ���� -> ó�� ��� ���� -> ���� -> ó�� ���� ������
// ó�� ����� �����ϰ� �����ϴ� ������ ������ �������̽��� ����
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
		
		static_assert(std::is_base_of_v<UEngineProtocol, Packet>, "��Ŷ Ÿ���� UEngineProtocol�� ����ؾ� �մϴ�");

		AGameMode* GameMode = Actor->GetWorld()->GetGameMode();
		if (nullptr == GameMode->GetNetwork())
		{
			return;
		}

		// Tick���� �Ź� SendPacket�� ������ ��Ŷ�� ���� �� ���� ������ ��Ŷ�� �����Ѵ�.
		// ���� 1ȸ ��Ŷ�� ������ ��� �� ������ ����
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
			MSGASSERT("��Ŷ ���� ����");
			return;
		}

		_SyncFunction(NewPacket);

		GameMode->GetNetwork()->SendPacket(NewPacket);
	}

	// �̷��� ¥�°� �մ����� �� �� ����غ���...
	// ȣ�� ����� NetObject�� ��ӹ��� ���͸��� ȣ���� �� �ִµ� ��Ŷ�� ó���ϴ� ����� �׻� ��Ʈ��ũ ������Ʈ�� ���ؼ� 
	// �Լ��� ����ϴ� ����� ������?
	
	//template<typename Packet, typename PacketType>
	//void ProcPacket(PacketType _Type, std::function<void(std::shared_ptr<Packet>)> _SyncFunction)
	//{
	//	static_assert(std::is_base_of_v<UEngineProtocol, Packet>, "��Ŷ Ÿ���� UEngineProtocol�� ����ؾ� �մϴ�");

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
	//			Network->SendPacket(_Packet); // �ٸ� Ŭ�󿡰Ե� ����
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

