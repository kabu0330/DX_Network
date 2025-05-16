#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// 셰이더에 상수 버퍼, 샘플러, 텍스처 바인딩

// 설명 : 셰이더 슬롯 바인딩 베이스 클래스
class UShaderSlotBindingBase 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_SHADER_TYPE;
	UINT BindIndex = 0;
};

// 설명 : 상수 버퍼 바인딩 정보
class UShaderConstantBufferBinding : public UShaderSlotBindingBase
{
public:
	void* Data = nullptr;
	UINT BufferSize = 0;
	std::shared_ptr<UEngineConstantBuffer> EngineConstantBuffer;

	void BindToShaderSlot()
	{
		if (nullptr != Data)
		{
			EngineConstantBuffer->UpdateConstantBufferData(Data, BufferSize);
		}

		EngineConstantBuffer->BindToShaderSlot(ShaderType, BindIndex);
	}

	void UnbindFromShaderSlot() {}
};

// 설명 : Texture2D 바인딩 정보
class UShaderTextureBinding : public UShaderSlotBindingBase
{
public:
	UEngineTexture* EngineTexture;

	void BindToShaderSlot()
	{
		EngineTexture->BindToShaderSlot(ShaderType, BindIndex);
	}

	void UnbindFromShaderSlot()
	{
		EngineTexture->UnbindFromShaderSlot(ShaderType, BindIndex);
	}
};

// 설명 : Sampler 바인딩 정보
class UShaderSamplerBinding : public UShaderSlotBindingBase
{
public:
	std::shared_ptr<UEngineSampler> EngineSampler;

	void BindToShaderSlot()
	{
		EngineSampler->BindToShaderSlot(ShaderType, BindIndex);
	}

	void UnbindFromShaderSlot()
	{
		EngineSampler->UnbindFromShaderSlot(ShaderType, BindIndex);
	}
};

// 설명 : 모든 셰이더 리소스(상수 버퍼, 텍스처, 샘플러) 바인딩을 관리하는 최종 관리자
class UShaderBindingManager
{
public:
	UShaderBindingManager();
	~UShaderBindingManager();

	void AddConstantBufferBinding(std::string_view _Name, UShaderConstantBufferBinding Res);
	void AddTextureBinding(std::string_view _Name, UShaderTextureBinding _Res);
	void AddSamplerBinding(std::string_view _Name, UShaderSamplerBinding _Res);


	template<typename DataType>
	void LinkConstantBufferData(std::string_view _Name, DataType& Data)
	{
		LinkConstantBufferData(_Name, reinterpret_cast<void*>(&Data));
	}
	void LinkConstantBufferData(std::string_view _Name, void* Data);

	void RegisterTextureBinding(std::string_view _Name, std::string_view _ResName);
	void RegisterTextureBinding(std::string_view _Name, UEngineTexture* _Texture);
	void RegisterTextureBinding(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);


	void RegisterSamplerBinding(std::string_view _Name, std::string_view _ResName);

	bool HasConstantBuffer(std::string_view _Name);
	bool HasTexture(std::string_view _Name);
	bool HasSampler(std::string_view _Name);

	void BindAllShaderSlots();
	void UnbindAllShaderSlots();

protected:

private:
	std::map<std::string, UShaderConstantBufferBinding> ConstantBufferSlots;
	std::map<std::string, UShaderTextureBinding> TextureSlots;
	std::map<std::string, UShaderSamplerBinding> SamplerSlots;
};

