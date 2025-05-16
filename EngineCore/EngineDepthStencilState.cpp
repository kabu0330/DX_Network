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
		MSGASSERT("[이름 중복]\n 뎁스 스텐실 스테이트 이름을 변경해주세요. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineDepthStencilState> NewDepthStencilState = std::make_shared<UEngineDepthStencilState>();
	AddResource<UEngineDepthStencilState>(NewDepthStencilState, _Name, "");
	NewDepthStencilState->CreateDepthStencilState(_Value);

	return NewDepthStencilState;
}

void UEngineDepthStencilState::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateDepthStencilState(&_Value, &DepthStencilState))
	{
		MSGASSERT("뎁스 스텐실 스테이트 생성에 실패했습니다.");
		return;
	}	
}

void UEngineDepthStencilState::OMSetDepthStencilState()
{
	// Omset 최종 픽셀테스트에 영향을 준다.
	UEngineCore::GetDevice().GetContext()->OMSetDepthStencilState(DepthStencilState.Get(), 0);
}
