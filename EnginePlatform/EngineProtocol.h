#pragma once
#include <memory>

#include <EngineBase/EngineSerializer.h>
#include <EngineBase/EngineMath.h>

enum EEnginePacketType
{
	UserAccessPacket = -2, // 클라와 서버가 정상적인 통신이 가능한지 확인하기 위해서 서로 교환하는 패킷
};

// 설명 :
class UEngineProtocol : public std::enable_shared_from_this<UEngineProtocol>, public ISerializeObject
{
public:
	void SerializePacket(UEngineSerializer& _Ser)
	{
		Serialize(_Ser);
		int* PacketSizeAddress = _Ser.ReadDataPtrAt<int>(4);
		*PacketSizeAddress = _Ser.GetWritePos();
	}

	void Serialize(UEngineSerializer& _Ser) override
	{
		_Ser << PacketType;
		_Ser << PacketSize;
		_Ser << SessionToken;
		_Ser << ObjectToken;
	}
	
	void DeSerialize(UEngineSerializer& _Ser) override
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

	static int GetProtocolSize()
	{
		return sizeof(int) * 4;
	}

protected:

private:
	int PacketType = -1;	// ex : 이동패킷, 공격패킷, 채팅패킷
	int PacketSize = -1;	// SerializePacket() 함수로 프로토콜 사이즈 알아서 넣어줄 것임
	int SessionToken = -1;  // 서버가 유저에게 부여한 ID(소켓)
	int ObjectToken = -1;	// 게임 내 오브젝트 식별 ID (처리 대상)

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