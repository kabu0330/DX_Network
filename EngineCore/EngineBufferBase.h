#pragma once

// 설명 : 버퍼와 버퍼 DESC 구조체를 멤버로 가진 클래스
//        상수 버퍼를 만들기 위해서는 반드시 상속받아야 한다.
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

