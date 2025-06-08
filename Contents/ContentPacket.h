#pragma once
#include <EnginePlatform/EngineProtocol.h>
#include "ContentPacketType.h"
#include "ContentEnum.h"

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
	FVector GetRotation() const
	{
		return Rotation;
	}

private:
	FVector Position = FVector::ZERO;
	FVector Rotation = FVector::ZERO;
};

class UMinoUpdatePacket : public UEngineProtocol
{
public:
	UMinoUpdatePacket()
	{
		SetPacketType(EContentsPacketType::MINO_UPDATE);
	}

	void Serialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::Serialize(_Ser);
		_Ser << static_cast<int>(MinoType);
	}
	void DeSerialize(UEngineSerializer& _Ser) override
	{
		int Type = 0;

		UEngineProtocol::DeSerialize(_Ser);
		_Ser >> Type;

		MinoType = static_cast<EMinoType>(Type);
	}

	void SetMinoType(EMinoType _MinoType)
	{
		MinoType = _MinoType;
	}
	EMinoType GetMinoType() const
	{
		return MinoType;
	}

private:
	EMinoType MinoType = EMinoType::MAX;
};

class USpawnPacket : public UEngineProtocol
{
public:
	USpawnPacket()
	{
		SetPacketType(EContentsPacketType::MINO_UPDATE);
	}

	void Serialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::Serialize(_Ser);
		_Ser << SubclassType;
		_Ser << SpawnObjectToken;
		_Ser << InitPos;
	}
	void DeSerialize(UEngineSerializer& _Ser) override
	{
		UEngineProtocol::DeSerialize(_Ser);
		_Ser >> SubclassType;
		_Ser >> SpawnObjectToken;
		_Ser >> InitPos;
	}

	void SetSubclassType(ETypeInfo _SubclassType)
	{
		SetSubclassType(static_cast<int>(_SubclassType));
	}
	void SetSubclassType(int _SubclassType)
	{
		SubclassType = _SubclassType;
	}
	int GetSubclassType() const
	{
		return SubclassType;
	}

	void SetSpawnObjectToken(int _ObjectToken)
	{
		SpawnObjectToken = _ObjectToken;
	}
	int GetSpawnObjectToken() const
	{
		return SpawnObjectToken;
	}

	void SetInitPos(FVector _Pos)
	{
		InitPos = _Pos;
	}
	FVector GetInitPos() const
	{
		return InitPos;
	}

private:
	int SubclassType = -1;
	int SpawnObjectToken = -1;
	FVector InitPos = FVector::ZERO;
};

class UChatPacket : public UEngineProtocol
{
public:
	UChatPacket()
	{
		//SetPacketType(EContentsPacketType::CHAT);
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

