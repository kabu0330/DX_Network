#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

// 설명 : 메모리 블록 읽기/쓰기
class UEngineSerializer
{
public:
	ENGINEAPI UEngineSerializer();
	ENGINEAPI ~UEngineSerializer();

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

	char* GetDataCharPtr()
	{
		return &Data[0];
	}

	char* GetWritePointer()
	{
		return &Data[WritePos];
	}

	template<typename T>
	T ReadDataAt(int _Offest = 0)
	{
		T* Result = reinterpret_cast<T*>(&Data[_Offest]);
		return *Result;
	}

	template<typename T>
	T* ReadDataPtrAt(int _Offset = 0)
	{
		T* Result = reinterpret_cast<T*>(&Data[_Offset]);
		return Result;
	}

	void AddWritePos(int _Size)
	{
		WritePos += _Size;
	}

	int GetWritePos()
	{
		return WritePos;
	}

	int GetReadPos()
	{
		return ReadPos;
	}

	int GetWritableBites()
	{
		return static_cast<int>(Data.size() - WritePos);
	}

	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

	size_t GetDataSize()
	{
		return Data.size();
	}

	void ResetReadPos()
	{
		ReadPos = 0;
	}

	// 읽은 데이터는 버리고 안 읽은 데이터를 앞으로 당긴다.
	ENGINEAPI void CompactBuffer();

protected:

private:
	int WritePos = 0;
	int ReadPos = 0;

	std::vector<char> Data; 

private:
	// delete Function
	UEngineSerializer(const UEngineSerializer& _Other) = delete;
	UEngineSerializer(UEngineSerializer&& _Other) noexcept = delete;
	UEngineSerializer& operator=(const UEngineSerializer& _Other) = delete;
	UEngineSerializer& operator=(UEngineSerializer&& _Other) noexcept = delete;
};

class ISerializeObject
{
public:
	ENGINEAPI ISerializeObject() {}
	ENGINEAPI virtual ~ISerializeObject() = 0
	{

	}

	ENGINEAPI virtual void Serialize(UEngineSerializer& _Ser); 	// 데이터 직렬화(압축)

	ENGINEAPI virtual void DeSerialize(UEngineSerializer& _Ser); // 데이터 복구
};
