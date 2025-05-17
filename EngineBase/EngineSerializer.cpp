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

void UEngineSerializer::Write(const void* _Data, unsigned int _Size)
{
	if (WritePos + _Size >= Data.size())
	{
		Data.resize(Data.size() * 2 + _Size);
	}

	memcpy_s(&Data[WritePos], _Size, _Data, _Size);
	WritePos += _Size;
}

void UEngineSerializer::operator<<(ISerializeObject& _Data)
{
	_Data.Serialize(*this);
}

void UEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	memcpy_s(_Data, _Size, &Data[ReadPos], _Size);
	ReadPos += _Size;
}

void UEngineSerializer::operator>>(ISerializeObject& _Data)
{
	_Data.DeSerialize(*this);
}

void UEngineSerializer::CompactBuffer()
{
	int RemainSize = WritePos - ReadPos;
	memcpy_s(&Data[0], RemainSize, &Data[ReadPos], RemainSize);
	WritePos = RemainSize;
	ReadPos = 0;
}

UEngineSerializer::UEngineSerializer()
{
}

UEngineSerializer::~UEngineSerializer()
{
}