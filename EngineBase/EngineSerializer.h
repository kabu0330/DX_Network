#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// 설명 :
class UEngineSerializer
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineSerializer();
	ENGINEAPI ~UEngineSerializer();

	// delete Function
	UEngineSerializer(const UEngineSerializer& _Other) = delete;
	UEngineSerializer(UEngineSerializer&& _Other) noexcept = delete;
	UEngineSerializer& operator=(const UEngineSerializer& _Other) = delete;
	UEngineSerializer& operator=(UEngineSerializer&& _Other) noexcept = delete;

	// 데이터의 크기
	ENGINEAPI void Write(const void* _Data, unsigned int _Size);

	ENGINEAPI void operator<<(const int& _Data)
	{
		Write(&_Data, sizeof(int));
	}

	ENGINEAPI void operator<<(const bool& _Data)
	{
		Write(&_Data, sizeof(bool));
	}

	ENGINEAPI void operator<<(const FVector& _Data)
	{
		Write(&_Data, sizeof(FVector));
	}

	ENGINEAPI void operator<<(const FIntPoint& _Data)
	{
		Write(&_Data, sizeof(FIntPoint));
	}

	ENGINEAPI void operator<<(const FTransform& _Data)
	{
		Write(&_Data, sizeof(FTransform));
	}

	ENGINEAPI void operator<<(const std::string& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		operator<<(Size);
		if (0 != Size)
		{
			Write(&_Data[0], static_cast<int>(_Data.size()));
		}
	}

	ENGINEAPI void operator<<(class ISerializeObject& _Data);

	template<typename DataType>
	ENGINEAPI void operator<<(std::vector<DataType>& _vector)
	{
		int Size = static_cast<int>(_vector.size());
		operator<<(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator<<(_vector[i]);
		}
	}

	ENGINEAPI void Read(void* _Data, unsigned int _Size);

	ENGINEAPI void operator>>(int& _Data)
	{
		Read(&_Data, sizeof(int));
	}

	ENGINEAPI void operator>>(bool& _Data)
	{
		Read(&_Data, sizeof(bool));
	}

	ENGINEAPI void operator>>(FVector& _Data)
	{
		Read(&_Data, sizeof(FVector));
	}

	ENGINEAPI void operator>>(FIntPoint& _Data)
	{
		Read(&_Data, sizeof(FIntPoint));
	}

	ENGINEAPI void operator>>(FTransform& _Data)
	{
		Read(&_Data, sizeof(FTransform));
	}

	ENGINEAPI void operator>>(std::string& _Data)
	{
		int Size;
		operator>>(Size);
		_Data.resize(Size);

		Read(&_Data[0], static_cast<int>(_Data.size()));
	}

	ENGINEAPI void operator>>(class ISerializeObject& _Data);

	template<typename DataType>
	ENGINEAPI void operator>>(std::vector<DataType>& _vector)
	{
		int Size = 0;
		operator>>(Size);
		_vector.resize(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator>>(_vector[i]);
		}
	}

	void* GetDataPtr()
	{
		return &Data[0];
	}

	int GetWriteOffset()
	{
		return WriteOffset;
	}

	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

	size_t GetDataSize()
	{
		return Data.size();
	}

	void ResetOffset()
	{
		ReadOffset = 0;
	}

protected:

private:
	int WriteOffset = 0;
	int ReadOffset = 0;

	std::vector<char> Data;
};

class ISerializeObject
{
public:
	ENGINEAPI ISerializeObject() {}
	ENGINEAPI virtual ~ISerializeObject() = 0
	{

	}

	// 데이터를 직렬화(압축)
	ENGINEAPI virtual void Serialize(UEngineSerializer& _Ser);
	// 데이터를 복구(할때)
	ENGINEAPI virtual void DeSerialize(UEngineSerializer& _Ser);
};
