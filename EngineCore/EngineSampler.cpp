#include "PreCompile.h"
#include "EngineSampler.h"
#include "EngineCore.h"

UEngineSampler::UEngineSampler()
{
}

UEngineSampler::~UEngineSampler()
{
}

std::shared_ptr<UEngineSampler> UEngineSampler::Create(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복]\n 샘플러 이름을 변경해주세요." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineSampler> NewSampler = std::make_shared<UEngineSampler>();
	PushResource<UEngineSampler>(NewSampler, _Name, "");
	NewSampler->CreateSamplerState(_Value);

	return NewSampler;
}

void UEngineSampler::CreateSamplerState(const D3D11_SAMPLER_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&_Value, &State))
	{
		MSGASSERT("샘플러 스테이트 생성에 실패했습니다.");
		return;
	}
}

void UEngineSampler::BindToShaderSlot(EShaderType _Type, UINT _BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { State.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetSamplers(_BindIndex, 1, ArrPtr);
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

void UEngineSampler::UnbindFromShaderSlot(EShaderType _Type, UINT _BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { nullptr };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetSamplers(_BindIndex, 1, ArrPtr);
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
