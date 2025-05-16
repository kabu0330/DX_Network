#include "PreCompile.h"
#include "EngineShader.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineConstantBuffer.h"

UEngineShader::UEngineShader()
{
}

UEngineShader::~UEngineShader()
{
}

void UEngineShader::AutoCompileShaderByNaming(UEngineFile& _File)
{
	_File.FileOpen("rt"); // �б� ����
	std::string ShaderCode = _File.GetAllFileText();
	
	// �� �� �̸� ��Ģ���� VS�� PS�� �����Ѵ�.
	{
		size_t EntryIndex = ShaderCode.find("_VS(");

		if (EntryIndex != std::string::npos)
		{
			{
				// �������� ã�Ƴ����� �Լ�
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_VS";

				UEngineVertexShader::CreateVertexShader(_File.GetPathToString(), EntryName);
				
			}
		}
	}
	{
		size_t EntryIndex = ShaderCode.find("_PS(");

		if (EntryIndex != std::string::npos)
		{
			{
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";

				UEnginePixelShader::CreatePixelShader(_File.GetPathToString(), EntryName);
			}
		}
	}
}

void UEngineShader::ReflectAndBindShaderResources()
{
	if (nullptr == ShaderCodeBlob)
	{
		MSGASSERT("���̴� �ڵ� ����� nullptr�Դϴ�.");
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> CompileInfo = nullptr;

	// RTTI
	if (S_OK != D3DReflect(ShaderCodeBlob->GetBufferPointer(), ShaderCodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &CompileInfo))
	{
		MSGASSERT("���÷��ǿ� �����߽��ϴ�.");
		return;
	}

	D3D11_SHADER_DESC Info;
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;

	// ������� �ؽ�ó ������ �ʰ� �׳� ���ҽ� ���� Info.BoundResources
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string Name = ResDesc.Name;
		std::string UpperName = UEngineString::ToUpper(Name);

		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* Info = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferInfo = { 0 };
			Info->GetDesc(&BufferInfo);

			std::shared_ptr<UEngineConstantBuffer> Buffer = UEngineConstantBuffer::CreateOrFind(BufferInfo.Size, UpperName);

			UShaderConstantBufferBinding NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.EngineConstantBuffer = Buffer;
			NewRes.BufferSize = BufferInfo.Size;
			ShaderBindingManager.AddConstantBufferBinding(UpperName, NewRes);
			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<UEngineTexture> Res = UEngineTexture::Find<UEngineTexture>("NSBase.png");

			UShaderTextureBinding NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.EngineTexture = Res.get();

			ShaderBindingManager.AddTextureBinding(UpperName, NewRes);

			break;
		}
		case D3D_SIT_SAMPLER:
		{
			std::shared_ptr<UEngineSampler> Res = UEngineSampler::Find<UEngineSampler>("WRapSampler");

			UShaderSamplerBinding NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.EngineSampler = Res;

			ShaderBindingManager.AddSamplerBinding(UpperName, NewRes);

			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			break;
		}
		case D3D_SIT_UAV_RWSTRUCTURED: // ��ǻƮ
		{
			break;
		}
		default:
			break;
		}
	}

	// 
	EntryName;
	ShaderBindingManager;

}



