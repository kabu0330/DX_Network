#include "PreCompile.h"
#include "EngineConstantBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> UEngineConstantBuffer::BufferMap;

void UEngineConstantBuffer::Release()
{
	BufferMap.clear();
}

UEngineConstantBuffer::UEngineConstantBuffer()
{
}

UEngineConstantBuffer::~UEngineConstantBuffer()
{
}

std::shared_ptr<UEngineConstantBuffer> UEngineConstantBuffer::CreateOrFind(UINT _Byte, const std::string_view& _Name)
{
	if (0 == _Byte) // 어떻게 0바이트에 데이터를 저장할 수 있겠어?
	{
		MSGASSERT("0바이트 상수버퍼는 만들 수 없습니다.");
		return nullptr;
	}

	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == BufferMap.contains(_Byte)) // 내가 알고 있는 바이트야?
	{
		if (true == BufferMap[_Byte].contains(UpperName)) // 내가 알고 있는 바이트에 이름이야?
		{
			return BufferMap[_Byte][UpperName]; // 그러면 내가 알고있는 상수버퍼 리턴
		}
	} // 그게 아니면 내가 모르는 상수버퍼야. 새로 만들자.

	std::shared_ptr<UEngineConstantBuffer> NewRes = std::make_shared<UEngineConstantBuffer>();
	NewRes->SetName(UpperName);
	NewRes->ResCreate(_Byte); // 상수 버퍼 생성
	BufferMap[_Byte][UpperName] = NewRes;

	return NewRes;
}

// 상수 버퍼 생성, 상수버퍼의 특징은 CPU가 동적으로 계산해준 데이터를 그래픽카드가 읽는 것
void UEngineConstantBuffer::ResCreate(UINT _Byte)
{
	BufferInfo.ByteWidth = _Byte;
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
	{
		MSGASSERT("상수버퍼 생성에 실패했습니다.");
		return;
	}
	
}

void UEngineConstantBuffer::ChangeData(void* _Data, UINT _Size)
{
	if (_Size != BufferInfo.ByteWidth)
	{
		MSGASSERT("바이트 크기가 다르게 세팅되었습니다" + GetName());
		return;
	}

	// FTransform& RendererTrans = GetTransformRef();
	D3D11_MAPPED_SUBRESOURCE Data = {};
	// 이 데이터를 사용하는 랜더링 랜더링 잠깐 정지
	// 잠깐 그래픽카드야 멈 그래픽카드에 있는 상수버퍼 수정해야 해.
	UEngineCore::GetDevice().GetContext()->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

	// Data.pData 그래픽카드와 연결된 주소값.
	if (nullptr == Data.pData)
	{
		MSGASSERT("그래픽카드가 수정을 거부했습니다.");
	}
	memcpy_s(Data.pData, BufferInfo.ByteWidth, _Data, BufferInfo.ByteWidth);
	UEngineCore::GetDevice().GetContext()->Unmap(Buffer.Get(), 0);
}

void UEngineConstantBuffer::Setting(EShaderType _Type, UINT _BindIndex)
{
	// 같은 상수버퍼를 
	ID3D11Buffer* ArrPtr[1] = { Buffer.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetConstantBuffers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetConstantBuffers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("아직 존재하지 않는 쉐이더에 세팅하려고 했습니다.");
		break;
	}
}
