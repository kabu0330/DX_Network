#include "PreCompile.h"
#include "EngineMaterial.h"

UEngineMaterial::UEngineMaterial()
{
	// 이미 만들어진 것들을 저장하는 역할
	SetRasterizerState("EngineBase");
	SetBlend("AlphaBlend");
	SetDepthStencilState("BaseDepth");
}

UEngineMaterial::~UEngineMaterial()
{
}

void UEngineMaterial::SetVertexShader(std::string_view _Name)
{
	VertexShader = UEngineVertexShader::Find<UEngineVertexShader>(_Name);

	if (nullptr == VertexShader)
	{
		MSGASSERT("[머티리얼 VS 연결 실패] \n 해당 버텍스 셰이더가 존재하지 않습니다. 셰이더 이름을 확인해주세요.");
		return;
	}
}


void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::Find<UEnginePixelShader>(_Name);

	if (nullptr == PixelShader)
	{
		MSGASSERT("[머티리얼 PS 연결 실패] \n 해당 픽셀 셰이더가 존재하지 않습니다. 셰이더 이름을 확인해주세요.");
		return;
	}
}

void UEngineMaterial::SetRasterizerState(std::string_view _Name)
{
	RasterizerState = UEngineRasterizerState::Find<UEngineRasterizerState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("[머티리얼 RS 연결 실패] \n 해당 레스터라이저가 존재하지 않습니다. 이름을 확인해주세요.");
	}
}

void UEngineMaterial::SetDepthStencilState(std::string_view _Name)
{
	DepthState = UEngineDepthStencilState::Find<UEngineDepthStencilState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("[머티리얼 뎁스 스테이트 연결 실패]\n 해당 뎁스 스테이트가 존재하지 않습니다. 이름을 확인해주세요.");
	}
}

void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::Find<UEngineBlend>(_Name);

	if (nullptr == Blend)
	{
		MSGASSERT("[머리티얼 블렌드 연결 실패]\n 해당 블렌드가 존재하지 않습니다. 이름을 확인해주세요.");
	}
}

// 머티리얼 메모리 블록을 생성하고, EngineResoure에서 관리된다.
std::shared_ptr<UEngineMaterial> UEngineMaterial::Create(std::string_view _Name)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복] \n 머티리얼 이름을 변경해주세요. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineMaterial> NewMaterial = std::make_shared<UEngineMaterial>();
	PushResource<UEngineMaterial>(NewMaterial, _Name, "");
	return NewMaterial; 
}

void UEngineMaterial::PrimitiveTopologySetting()
{
	UEngineCore::GetDevice().GetContext()->IASetPrimitiveTopology(TOPOLOGY);
}