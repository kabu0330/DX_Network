#include "PreCompile.h"
#include "EngineDepthStencilState.h"
#include "EngineCore.h"

UEngineDepthStencilState::UEngineDepthStencilState()
{
}

UEngineDepthStencilState::~UEngineDepthStencilState()
{
}



std::shared_ptr<UEngineDepthStencilState> UEngineDepthStencilState::Create(std::string_view _Name, const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineDepthStencilState> NewRes = std::make_shared<UEngineDepthStencilState>();
	PushRes<UEngineDepthStencilState>(NewRes, _Name, "");
	NewRes->CreateViewObject(_Value);

	return NewRes;
}


void UEngineDepthStencilState::CreateViewObject(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateDepthStencilState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
	
}

void UEngineDepthStencilState::Setting()
{
	// Omset 최종 픽셀테스트에 영향을 준다.
	UEngineCore::GetDevice().GetContext()->OMSetDepthStencilState(State.Get(), 0);
}
