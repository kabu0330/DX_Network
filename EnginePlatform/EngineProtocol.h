#pragma once
#include <memory>

#include <EngineBase/EngineSerializer.h>
#include <EngineBase/EngineMath.h>

enum EEnginePacketType
{
	UserAccessPacket = -2, // Ŭ��� ������ �������� ����� �������� Ȯ���ϱ� ���ؼ� ���� ��ȯ�ϴ� ��Ŷ
};

// ���� :
class UEngineProtocol : public std::enable_shared_from_this<UEngineProtocol>, public ISerializeObject
{
public:
	void SerializePacket(UEngineSerializer& _Ser)
	{
		Serialize(_Ser);
		int* PacketSize = _Ser.SeekDataPtr<int>(4);
		*PacketSize = _Ser.GetWriteOffset();
	}

	void Serialize(UEngineSerializer& _Ser)
	{
		_Ser << PacketType;
		_Ser << PacketSize;
		_Ser << SessionToken;
		_Ser << ObjectToken;
	}
	
	void DeSerialize(UEngineSerializer& _Ser)
	{
		_Ser >> PacketType;
		_Ser >> PacketSize;
		_Ser >> SessionToken;
		_Ser >> ObjectToken;
	}


	template<typename TPacketType>
	TPacketType GetPacketType()
	{
		return static_cast<TPacketType>(PacketType);
	}
	int GetPacketType()   { return PacketType; }
	int GetPacketSize()   { return PacketSize; }
	int GetSessionToken() { return SessionToken; }
	int GetObjectToken()  { return ObjectToken; }
	
	template<typename TPacketType>
	void SetPacketType(TPacketType _Value)
	{
		PacketType = static_cast<int>(_Value);
	}
	void SetPacketType(int _Value) 
	{ 
		PacketType = _Value; 
	}
	void SetPacketSize(int _Size) 
	{
		PacketSize = _Size; 
	}
	void SetSessionToken(int _Value) 
	{ 
		SessionToken = _Value; 
	
	}
	void SetObjectToken(int _Value)
	{
		ObjectToken = _Value;
	}

protected:

private:
	int PacketType = -1; // ex : �̵���Ŷ, ������Ŷ, ä����Ŷ
	int PacketSize = -1;
	int SessionToken = -1; // ������ �������� �ο��� ID
	int ObjectToken = -1; // ���� �� ������Ʈ �ĺ� ID (ó�� ���)

private:

};

class UUserAccessPacket : public UEngineProtocol
{
public: 
	UUserAccessPacket()
	{
		SetPacketType(EEnginePacketType::UserAccessPacket);
	}
};