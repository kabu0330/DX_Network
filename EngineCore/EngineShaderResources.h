#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// 설명 : 이름, 셰이더 타입, 인덱스
class UEngineShaderRes 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_SHADER_TYPE;
	UINT BindIndex = 0;
};

// 설명 : 상수버퍼 리소스는 셰이더 리소스 + 버퍼 사이즈 + 상수 버퍼
// 상수버퍼에 세팅을 위해서 각자가 가져야할 값들을 저장하기 위한 클래스
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
		Res->Reset(ShaderType, BindIndex);
	}
};

// 설명 : 상수버퍼, 텍스처, 샘플러 관리
class UEngineShaderResources
{
public:
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
};

