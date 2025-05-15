#pragma once

// ���� : ���� ����ü(FEngineVertex)�� ��� ������ ���̴��� �Է� �ø�ƽ�� �����ϴ� InputLayout Ŭ����
//		  ���̴����� ���ε��� ���� ���̾ƿ� ���� ����
class UEngineInputLayoutInfo
{
public:
	ENGINEAPI UEngineInputLayoutInfo();
	ENGINEAPI ~UEngineInputLayoutInfo();

	ENGINEAPI void AddInputLayout(
		LPCSTR _SemanticName,														// ���� �������̴� ���� �÷���. "POSITION"
		DXGI_FORMAT _Format,														// ���� n����Ʈ ¥�� ������ �ڷ���
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,   // ���ؽ� �����Ͱ� �ν��Ͻ� �����ʹ� ��°� ǥ��
		UINT _InstanceDataStepRate = 0,												// 
		UINT _AlignedByteOffset = -1,												// 0��° ����Ʈ���� 
		UINT _InputSlot = 0,														// n���� ���ؽ� ���۸� �����Ҷ� n��° ���ؽ� ������ ��ǲ ���̾ƿ��̴� ��°� ǥ���ϴ°�
		UINT _SemanticIndex = 0
	);

	ENGINEAPI UINT FormatSize(DXGI_FORMAT _Value);

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDescs;

protected:

private:
	int CurrentByteOffset = 0;

private:
	// delete Function
	UEngineInputLayoutInfo(const UEngineInputLayoutInfo& _Other) = delete;
	UEngineInputLayoutInfo(UEngineInputLayoutInfo&& _Other) noexcept = delete;
	UEngineInputLayoutInfo& operator=(const UEngineInputLayoutInfo& _Other) = delete;
	UEngineInputLayoutInfo& operator=(UEngineInputLayoutInfo&& _Other) noexcept = delete;
};

