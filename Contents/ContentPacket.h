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
		_Ser << Rotation;
	}
	void DeSerialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::DeSerialize(_Ser);
		_Ser >> Position;
		_Ser >> Rotation;
	}

	void SetPosition(FVector _Position)
	{
		Position = _Position;
	}

	FVector GetPosition() const
	{
		return Position;
	}

	void SetRotation(FVector _Rotation)
	{
		Rotation = _Rotation;
	}
	FVector GetRotation()
	{
		return Rotation;
	}

private:
	FVector Position = FVector::ZERO;
	FVector Rotation = FVector::ZERO;
};


class UChatPacket : public UEngineProtocol
{
public:
	UChatPacket()
	{
		SetPacketType(EContentsPacketType::CHAT);
	}

	void Serialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::Serialize(_Ser);
		_Ser << Text;
	}
	void DeSerialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::DeSerialize(_Ser);
		_Ser >> Text;
	}

	void SetText(std::string_view _Text)
	{
		Text = _Text;
	}
	std::string GetText() const
	{
		return Text;
	}

private:
	std::string Text;
};

