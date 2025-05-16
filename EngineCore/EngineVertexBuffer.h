#pragma once
#include "EngineResourceManager.h"
#include "EngineBufferBase.h"
#include "EngineVertex.h"

// 설명 : 정점(Vertex) 정보를 GPU에 전달하는 DX11 버텍스 버퍼 클래스
class UEngineVertexBuffer : public UEngineResourceManager, public UEngineBufferBase
{
public:
	UEngineVertexBuffer();
	~UEngineVertexBuffer();

	template<typename VertexType>
	static std::shared_ptr<UEngineVertexBuffer> Create(std::string_view _Name, const std::vector<VertexType>& _VertexData)
	{
		return Create(_Name, reinterpret_cast<const void*>(&_VertexData[0]), sizeof(VertexType), _VertexData.size(), &VertexType::GetInfo());
	}

	static std::shared_ptr<UEngineVertexBuffer> Create(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount, UEngineInputLayoutInfo* _InfoPtr = nullptr);

	void IASetVertexBuffers();

	ENGINEAPI UEngineInputLayoutInfo* GetInputLayoutInfo()
	{
		return InputLayoutInfo;
	}

protected:
	void CreateVertexBuffer(const void* _InitData, size_t _VertexSize, size_t _VertexCount);

private:
	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UEngineInputLayoutInfo* InputLayoutInfo;

private:
	// delete Function
	UEngineVertexBuffer(const UEngineVertexBuffer& _Other) = delete;
	UEngineVertexBuffer(UEngineVertexBuffer&& _Other) noexcept = delete;
	UEngineVertexBuffer& operator=(const UEngineVertexBuffer& _Other) = delete;
	UEngineVertexBuffer& operator=(UEngineVertexBuffer&& _Other) noexcept = delete;
};

