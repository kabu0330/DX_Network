#include "PreCompile.h"
#include "EngineSerializer.h"

// 데이터를 직렬화(압축)
void ISerializeObject::Serialize(UEngineSerializer& _Ser)
{

}
// 데이터를 복구(할때)
void ISerializeObject::DeSerialize(UEngineSerializer& _Ser)
{

}

UEngineSerializer::UEngineSerializer()
{
}

UEngineSerializer::~UEngineSerializer()
{
}

void UEngineSerializer::Write(const void* _Data, unsigned int _Size)
{
	if (WriteOffset + _Size >= Data.size())
	{
		Data.resize(Data.size() * 2 + _Size);
	}

	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += _Size;
}

void UEngineSerializer::operator<<(ISerializeObject& _Data)
{
	_Data.Serialize(*this);
}

void UEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += _Size;
}

void UEngineSerializer::operator>>(ISerializeObject& _Data)
{
	_Data.DeSerialize(*this);
}
