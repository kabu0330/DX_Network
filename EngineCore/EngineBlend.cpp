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
		MSGASSERT("[�̸� �ߺ�]\n ���� �̸��� �������ּ���. " + UpperName);
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
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}

void UEngineBlend::OMSetBlendState()
{
	UEngineCore::GetDevice().GetContext()->OMSetBlendState(State.Get(), BlendFactor.Arr1D, Mask);
}
