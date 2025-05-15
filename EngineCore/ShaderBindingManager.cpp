#include "PreCompile.h"
#include "ShaderBindingManager.h"
#include "EngineBase/EngineDebug.h"

void UShaderBindingManager::AddSamplerBinding(std::string_view _Name, UShaderSamplerBinding _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == SamplerSlots.contains(UpperString))
	{
		MSGASSERT("[�̸� �ߺ�]\n �ٸ� �̸��� �Է����ּ���.");
		return;
	}

	SamplerSlots[UpperString] = _Res;
}

void UShaderBindingManager::AddTextureBinding(std::string_view _Name, UShaderTextureBinding _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == TextureSlots.contains(UpperString))
	{
		MSGASSERT("[�̸� �ߺ�]\n �ٸ� �̸��� �Է����ּ���.");
		return;
	}

	TextureSlots[UpperString] = _Res;
}

void UShaderBindingManager::AddConstantBufferBinding(std::string_view _Name, UShaderConstantBufferBinding _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == ConstantBufferSlots.contains(UpperString))
	{
		MSGASSERT("[�̸� �ߺ�]\n �ٸ� �̸��� �Է����ּ���.");
		return;
	}

	ConstantBufferSlots[UpperString] = _Res;
}

void UShaderBindingManager::BindAllShaderSlots()
{
	for (std::pair<const std::string, UShaderConstantBufferBinding>& ConstantBuffer : ConstantBufferSlots)
	{
		ConstantBuffer.second.BindToShaderSlot();
	}

	for (std::pair<const std::string, UShaderTextureBinding>& Texture : TextureSlots)
	{
		Texture.second.BindToShaderSlot();
	}

	for (std::pair<const std::string, UShaderSamplerBinding>& Sampler : SamplerSlots)
	{
		Sampler.second.BindToShaderSlot();
	}
}

bool UShaderBindingManager::HasSampler(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return SamplerSlots.contains(UpperName);
}

bool UShaderBindingManager::HasTexture(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return TextureSlots.contains(UpperName);
}
bool UShaderBindingManager::HasConstantBuffer(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return ConstantBufferSlots.contains(UpperName);
}

void UShaderBindingManager::LinkConstantBufferData(std::string_view _Name, void* _Data)
{
	std::string ConstantBufferUpperName = UEngineString::ToUpper(_Name);

	if (false == ConstantBufferSlots.contains(ConstantBufferUpperName))
	{
		UEngineDebug::OutPutString("�̹� ��ϵ� ��� ���� : " + ConstantBufferUpperName);
		return;
	}

	ConstantBufferSlots[ConstantBufferUpperName].Data = _Data;
}

void UShaderBindingManager::RegisterSamplerBinding(std::string_view _Name, std::string_view _ResName)
{
	std::string SamplerUpperName = UEngineString::ToUpper(_Name);

	if (false == SamplerSlots.contains(SamplerUpperName))
	{
		UEngineDebug::OutPutString("�̹� ��ϵ� ���÷� : " + SamplerUpperName);
		return;
	}

	SamplerSlots[SamplerUpperName].EngineSampler = UEngineSampler::Find<UEngineSampler>(_ResName);

}

void UShaderBindingManager::RegisterTextureBinding(std::string_view _Name, std::string_view _ResName)
{
	std::string TextureUpperName = UEngineString::ToUpper(_Name);

	if (false == TextureSlots.contains(TextureUpperName))
	{
		UEngineDebug::OutPutString("�̹� ��ϵ� �ؽ�ó : " + TextureUpperName);
		return;
	}

	TextureSlots[TextureUpperName].EngineTexture = UEngineTexture::Find<UEngineTexture>(_ResName);
}

void UShaderBindingManager::RegisterTextureBinding(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	std::string TextureUpperName = UEngineString::ToUpper(_Name);

	if (false == TextureSlots.contains(TextureUpperName))
	{
		UEngineDebug::OutPutString("�̹� ��ϵ� �ؽ�ó : " + TextureUpperName);
		return;
	}

	TextureSlots[TextureUpperName].EngineTexture = _Texture;
}

void UShaderBindingManager::UnbindAllShaderSlots()
{
	for (std::pair<const std::string, UShaderTextureBinding>& Texture : TextureSlots)
	{
		Texture.second.UnbindFromShaderSlot();
	}

	for (std::pair<const std::string, UShaderSamplerBinding>& Sampler : SamplerSlots)
	{
		Sampler.second.UnbindFromShaderSlot();
	}
}

UShaderBindingManager::UShaderBindingManager()
{
}

UShaderBindingManager::~UShaderBindingManager()
{
}