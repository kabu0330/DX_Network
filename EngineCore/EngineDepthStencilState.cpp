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
		MSGASSERT("[�̸� �ߺ�]\n ���� ���ٽ� ������Ʈ �̸��� �������ּ���. " + UpperName);
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
		MSGASSERT("���� ���ٽ� ������Ʈ ������ �����߽��ϴ�.");
		return;
	}	
}

void UEngineDepthStencilState::OMSetDepthStencilState()
{
	// Omset ���� �ȼ��׽�Ʈ�� ������ �ش�.
	UEngineCore::GetDevice().GetContext()->OMSetDepthStencilState(DepthStencilState.Get(), 0);
}
