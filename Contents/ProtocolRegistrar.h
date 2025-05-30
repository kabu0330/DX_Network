#pragma once
#include <EnginePlatform/EngineDispatcher.h>
#include "ContentPacketType.h"

#include <EnginePlatform/EngineNetwork.h>
#include "ContentPacket.h"
#include <EngineCore/NetObject.h>
#include "ServerPawn.h"


// Ό³Έν :
class UProtocolRegistrar
{
public:
	~UProtocolRegistrar();

	template<typename Protocol>
	static void AddProtocolHandler(class UEngineNetwork* _Net, EContentsPacketType _PacketType, class ULevel* _World)
	{
		_Net->GetDispatcher().AddHandler<Protocol>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE),
			[_World, _Net](std::shared_ptr<Protocol> _Packet)
			{
				int Token = _Packet->GetObjectToken();
				AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
				if (false == UNetObject::IsNetObject(Token))
				{
					std::shared_ptr<AServerPawn> NewServerPawn = _World->SpawnActor<AServerPawn>();
					ServerPawn = NewServerPawn.get();
					ServerPawn->SetControllOff();
					ServerPawn->InitNetObject(_Packet->GetObjectToken(), _Packet->GetSessionToken());
				}

				ServerPawn->SetActorLocation(_Packet->GetPosition());
				_Net->SendPacket(_Packet);
			});
	}

protected:

private:
	UProtocolRegistrar();

};

