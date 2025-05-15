#pragma once

// ���� : GPU�� �����Ǵ� ��� ����(Vertex, Index, Constant)�� ���� ��� Ŭ����
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

