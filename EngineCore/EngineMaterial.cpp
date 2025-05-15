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
		MSGASSERT("[��Ƽ���� VS ���� ����] \n �ش� ���ؽ� ���̴��� �������� �ʽ��ϴ�. ���̴� �̸��� Ȯ�����ּ���.");
		return;
	}
}


void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::Find<UEnginePixelShader>(_Name);

	if (nullptr == PixelShader)
	{
		MSGASSERT("[��Ƽ���� PS ���� ����] \n �ش� �ȼ� ���̴��� �������� �ʽ��ϴ�. ���̴� �̸��� Ȯ�����ּ���.");
		return;
	}
}

void UEngineMaterial::SetRasterizerState(std::string_view _Name)
{
	RasterizerState = UEngineRasterizerState::Find<UEngineRasterizerState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("[��Ƽ���� RS ���� ����] \n �ش� �����Ͷ������� �������� �ʽ��ϴ�. �̸��� Ȯ�����ּ���.");
	}
}

void UEngineMaterial::SetDepthStencilState(std::string_view _Name)
{
	DepthState = UEngineDepthStencilState::Find<UEngineDepthStencilState>(_Name);

	if (nullptr == RasterizerState)
	{
		MSGASSERT("[��Ƽ���� ���� ������Ʈ ���� ����]\n �ش� ���� ������Ʈ�� �������� �ʽ��ϴ�. �̸��� Ȯ�����ּ���.");
	}
}

void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::Find<UEngineBlend>(_Name);

	if (nullptr == Blend)
	{
		MSGASSERT("[�Ӹ�Ƽ�� ���� ���� ����]\n �ش� ���尡 �������� �ʽ��ϴ�. �̸��� Ȯ�����ּ���.");
	}
}

// ��Ƽ���� �޸� ����� �����ϰ�, EngineResoure���� �����ȴ�.
std::shared_ptr<UEngineMaterial> UEngineMaterial::Create(std::string_view _Name)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[�̸� �ߺ�] \n ��Ƽ���� �̸��� �������ּ���. " + UpperName);
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