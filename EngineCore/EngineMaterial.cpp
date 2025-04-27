#include "PreCompile.h"
#include "EngineMaterial.h"

UEngineMaterial::UEngineMaterial()
{
	// �̹� ������� �͵��� �����ϴ� ����
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
		MSGASSERT("�������� �ʴ� ���ؽ� ���̴��Դϴ�.");
	}
}


void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::Find<UEnginePixelShader>(_Name);

	if (nullptr == PixelShader)
	{
		MSGASSERT("�������� �ʴ� �ȼ� ���̴��Դϴ�.");
	}
}

void UEngineMaterial::SetRasterizerState(std::string_view _Name)
{
	RasterizerState = UEngineRasterizerState::Find<UEngineRasterizerState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("�������� �ʴ� �����Ͷ������Դϴ�.");
	}
}

void UEngineMaterial::SetDepthStencilState(std::string_view _Name)
{
	DepthState = UEngineDepthStencilState::Find<UEngineDepthStencilState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("�������� �ʴ� ���� ���ٽ��Դϴ�.");
	}
}

void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::Find<UEngineBlend>(_Name);

	if (nullptr == Blend)
	{
		MSGASSERT("�������� �ʴ� ���� �����Դϴ�.");
	}
}

// ��Ƽ���� �޸� ����� �����ϰ�, EngineResoure���� �����ȴ�.
std::shared_ptr<UEngineMaterial> UEngineMaterial::Create(std::string_view _Name)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� ���� �̸��� ��Ƽ������ �����մϴ�. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineMaterial> NewRes = std::make_shared<UEngineMaterial>();
	PushRes<UEngineMaterial>(NewRes, _Name, ""); // �ؽ�ó ������ ����
	return NewRes; 
}

void UEngineMaterial::PrimitiveTopologySetting()
{
	UEngineCore::GetDevice().GetContext()->IASetPrimitiveTopology(TOPOLOGY);
}