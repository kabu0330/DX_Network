#include "PreCompile.h"
#include "EngineRasterizerState.h"

UEngineRasterizerState::UEngineRasterizerState()
{
}

UEngineRasterizerState::~UEngineRasterizerState()
{
}

std::shared_ptr<UEngineRasterizerState> UEngineRasterizerState::Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복]\n 래스터라이저 스테이트의 이름을 변경하세요. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineRasterizerState> NewRes = std::make_shared<UEngineRasterizerState>();
	AddResource<UEngineRasterizerState>(NewRes, _Name, "");
	NewRes->CreateRasterizerState(_Value);

	return NewRes;
}


void UEngineRasterizerState::CreateRasterizerState(const D3D11_RASTERIZER_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&_Value, &RasterizerState))
	{
		MSGASSERT("래스터라이저 스테이트 생성에 실패했습니다");
		return;
	}
}

void UEngineRasterizerState::RSSetState()
{
	UEngineCore::GetDevice().GetContext()->RSSetState(RasterizerState.Get());
}
