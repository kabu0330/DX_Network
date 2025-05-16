#pragma once

// 설명 : 정점 구조체(FEngineVertex)의 멤버 정보를 셰이더의 입력 시맨틱과 연결하는 InputLayout 클래스
//		  셰이더와의 바인딩을 위한 레이아웃 정보 제공
class UEngineInputLayoutInfo
{
public:
	ENGINEAPI UEngineInputLayoutInfo();
	ENGINEAPI ~UEngineInputLayoutInfo();

	ENGINEAPI void AddInputLayout(
		LPCSTR _SemanticName,														// 내가 포지션이다 내가 컬러다. "POSITION"
		DXGI_FORMAT _Format,														// 내가 n바이트 짜리 정보다 자료형
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,   // 버텍스 데이터가 인스턴싱 데이터다 라는걸 표현
		UINT _InstanceDataStepRate = 0,												// 
		UINT _AlignedByteOffset = -1,												// 0번째 바이트부터 
		UINT _InputSlot = 0,														// n개의 버텍스 버퍼를 세팅할때 n번째 버텍스 버퍼의 인풋 레이아웃이다 라는걸 표현하는것
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

