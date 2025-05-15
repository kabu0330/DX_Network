#include "PreCompile.h"
#include "EngineBlend.h"
#include "EngineCore.h"

UEngineBlend::UEngineBlend()
{
}

UEngineBlend::~UEngineBlend()
{
}

std::shared_ptr<UEngineBlend> UEngineBlend::Create(std::string_view _Name, const D3D11_BLEND_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복]\n 블렌드 이름을 변경해주세요. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineBlend> NewBlend = std::make_shared<UEngineBlend>();
	PushResource<UEngineBlend>(NewBlend, _Name, "");
	NewBlend->CreateBlendState(_Value);

	return NewBlend;
}


void UEngineBlend::CreateBlendState(const D3D11_BLEND_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateBlendState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
}

void UEngineBlend::OMSetBlendState()
{
	UEngineCore::GetDevice().GetContext()->OMSetBlendState(State.Get(), BlendFactor.Arr1D, Mask);
}
