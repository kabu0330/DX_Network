#pragma once

// 설명 : GPU에 생성되는 모든 버퍼(Vertex, Index, Constant)의 공통 기반 클래스
class UEngineBufferBase
{
public:
	UEngineBufferBase();
	~UEngineBufferBase();

	const D3D11_BUFFER_DESC& GetBufferInfo()
	{
		return BufferInfo;
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };

private:
	// delete Function
	UEngineBufferBase(const UEngineBufferBase& _Other) = delete;
	UEngineBufferBase(UEngineBufferBase&& _Other) noexcept = delete;
	UEngineBufferBase& operator=(const UEngineBufferBase& _Other) = delete;
	UEngineBufferBase& operator=(UEngineBufferBase&& _Other) noexcept = delete;
};

