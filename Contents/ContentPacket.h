#pragma once
#include <EnginePlatform/EngineProtocol.h>
#include "ContentPacketType.h"

// Ό³Έν :
class UObjectUpdatePacket : public UEngineProtocol
{
public:
	UObjectUpdatePacket()
	{
		SetPacketType(EContentsPacketType::OBJECT_UPDATE);
	}

	void Serialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::Serialize(_Ser);
		_Ser << Position;
	}
	void DeSerialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::DeSerialize(_Ser);
		_Ser >> Position;
	}

	void SetPosition(FVector _Position)
	{
		Position = _Position;
	}

	FVector GetPosition() const
	{
		return Position;
	}

private:
	FVector Position = FVector::ZERO;
};

