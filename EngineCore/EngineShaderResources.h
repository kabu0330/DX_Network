#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// ���� : �̸�, ���̴� Ÿ��, �ε���
class UEngineShaderRes 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_SHADER_TYPE;
	UINT BindIndex = 0;
};

// ���� : ������� ���ҽ��� ���̴� ���ҽ� + ���� ������ + ��� ����
// ������ۿ� ������ ���ؼ� ���ڰ� �������� ������ �����ϱ� ���� Ŭ����
class UEngineConstantBufferRes : public UEngineShaderRes
{
public:
	void* Data = nullptr; // �ڽſ��� ���õ� �����ʹ� ������ ������ ���� ���̴�.
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

// ���� : �������, �ؽ�ó, ���÷� ����
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

