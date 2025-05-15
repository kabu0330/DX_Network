#include "PreCompile.h"
#include "EngineVertexBuffer.h"

UEngineVertexBuffer::UEngineVertexBuffer()
{
}

UEngineVertexBuffer::~UEngineVertexBuffer()
{
}

std::shared_ptr<UEngineVertexBuffer> UEngineVertexBuffer::Create(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount,
	UEngineInputLayoutInfo* _InfoPtr)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복] \n 버텍스 버퍼 이름을 변경해주세요." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineVertexBuffer> NewVertexBuffer = std::make_shared<UEngineVertexBuffer>();
	PushResource<UEngineVertexBuffer>(NewVertexBuffer, _Name, "");
	NewVertexBuffer->CreateViewObject(_InitData, _VertexSize, _VertexCount);
	NewVertexBuffer->InfoPtr = _InfoPtr;
	

	return NewVertexBuffer;
}

void UEngineVertexBuffer::CreateViewObject(const void* _InitData, size_t _VertexSize, size_t _VertexCount)
{
	VertexCount = static_cast<UINT>(_VertexCount);
	VertexSize = static_cast<UINT>(_VertexSize);
	BufferInfo.ByteWidth = static_cast<UINT>(_VertexSize * _VertexCount);
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // 초기값 넣어주는 용도의 구조체
	Data.pSysMem = _InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MSGASSERT("버텍스 버퍼 생성에 실패했습니다.");
		return;
	}
}

void UEngineVertexBuffer::Setting()
{
	// UINT VertexSize = sizeof(EngineVertex);
	// 이 버텍스 버퍼가 10개짜리인데 3번째 버텍스 부터 세팅해줘.
	UINT Offset = 0;
	ID3D11Buffer* ArrBuffer[1];
	ArrBuffer[0] = Buffer.Get();
	UEngineCore::GetDevice().GetContext()->IASetVertexBuffers(0, 1, ArrBuffer, &VertexSize, &Offset);
	
}