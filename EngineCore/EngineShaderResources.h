#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// 설명 : 셰이더 리소스는 이름과 셰이더 타입(VS, PS 등) 그리고 몇번째 인덱스인지를 하나의 데이터로 저장한다.
class UEngineShaderRes 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_ShaderType;
	UINT BindIndex = 0;
};

// 설명 : 상수버퍼 리소스는 셰이더 리소스 + 버퍼 사이즈 + 상수 버퍼
class UEngineConstantBufferRes : public UEngineShaderRes
{
public:
	void* Data = nullptr; // 자신에게 세팅될 데이터는 스스로 가지고 있을 것이다.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;

	void Setting()
	{
		if (nullptr != Data)
		{
			Name;
			Res->ChangeData(Data, BufferSize);
		}

		Res->Setting(ShaderType, BindIndex);
	}

	void Reset()
	{

	}
};

class UEngineTextureRes : public UEngineShaderRes
{
public:
	std::shared_ptr<UEngineTexture> Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}

	void Reset()
	{
		Res->Reset(ShaderType, BindIndex);
	}
};

class UEngineSamplerRes : public UEngineShaderRes
{
public:
	std::shared_ptr<UEngineSampler> Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}

	void Reset()
	{
		Res->Setting(ShaderType, BindIndex);
	}
};

// 설명 : 
class UEngineShaderResources
{
	// 

public:
	// constrcuter destructer
	UEngineShaderResources();
	~UEngineShaderResources();

	void CreateSamplerRes(std::string_view _Name, UEngineSamplerRes _Res);

	void CreateTextureRes(std::string_view _Name, UEngineTextureRes _Res);

	void CreateConstantBufferRes(std::string_view _Name, UEngineConstantBufferRes Res);

	template<typename DataType>
	void ConstantBufferLinkData(std::string_view _Name, DataType& Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&Data));
	}

	void ConstantBufferLinkData(std::string_view _Name, void* Data);

	void SamplerSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);


	bool IsSampler(std::string_view _Name);
	bool IsTexture(std::string_view _Name);
	bool IsConstantBuffer(std::string_view _Name);
	void Setting();

	void Reset();


protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
	std::map<std::string, UEngineTextureRes> TextureRes;
	std::map<std::string, UEngineSamplerRes> SamplerRes;
	// std::map<std::string, UEngineConstantBufferRes> ConstantBufferSetters;

};

