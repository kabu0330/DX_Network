#include "PreCompile.h"
#include "RenderUnit.h"
#include "EngineEnums.h"
#include "Renderer.h"

URenderUnit::URenderUnit()
{
}

URenderUnit::~URenderUnit()
{
}

void URenderUnit::InitializeShaderResources()
{
	if (nullptr == Material)
	{
		MSGASSERT("머티리얼이 존재하지 않습니다.");
		return;
	}

	{
		UShaderBindingManager& VS = Material->GetVertexShader()->ShaderResources;
		Resources[EShaderType::VS] = Material->GetVertexShader()->ShaderResources;
	}

	{
		UShaderBindingManager& PS = Material->GetPixelShader()->ShaderResources;
		Resources[EShaderType::PS] = Material->GetPixelShader()->ShaderResources;
	}

	// 부모 렌더러가 없다면, 그건 UI다.
	if (nullptr != ParentRenderer)
	{
		TransformObject = ParentRenderer; // UI는 트랜스폼 오브젝트는 가지고 있다.
	}

	if (nullptr != TransformObject) // 모든 셰이더를 돌면서
	{
		for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_SHADER_TYPE; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
		{
			if (false == Resources.contains(i))
			{
				continue;
			}

			if (false == Resources[i].HasConstantBuffer("FTransform"))
			{
				continue;
			}

			FTransform& Ref = TransformObject->GetTransformRef();
			Resources[i].LinkConstantBufferData("FTransform", Ref);
		}	
	}
}

void URenderUnit::ConstantBufferLinkData(std::string_view _Name, void* _Data)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_SHADER_TYPE; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].HasConstantBuffer(_Name))
		{
			continue;
		}

		Resources[i].LinkConstantBufferData(_Name, _Data);
	}
}

void URenderUnit::BindTextureToShaderSlotByName(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_SHADER_TYPE; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].HasTexture(_Name))
		{
			continue;
		}

		Resources[i].RegisterTextureBinding(_Name, _ResName);
	}
}

void URenderUnit::BindTextureToShaderSlot(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_SHADER_TYPE; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].HasTexture(_Name))
		{
			continue;
		}

		Resources[i].RegisterTextureBinding(_Name, _Texture);
	}
}

void URenderUnit::SetSampler(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_SHADER_TYPE; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].HasSampler(_Name))
		{
			continue;
		}

		Resources[i].RegisterSamplerBinding(_Name, _ResName);
	}
}

void URenderUnit::SetMesh(std::string_view _Name)
{
	Mesh = UMesh::Find<UMesh>(_Name);

	if (nullptr == Mesh)
	{
		std::string Name = _Name.data();
		MSGASSERT("존재하지 않는 메시입니다." + Name);
	}

	if (nullptr != Material)
	{
		CreateInputLayout();
	}
}

void URenderUnit::SetMaterial(std::string_view _Name)
{
	Material = UEngineMaterial::Find<UEngineMaterial>(_Name);

	if (nullptr == Material)
	{
		std::string Name = _Name.data();
		MSGASSERT(Name + " 머티리얼이 존재하지 않습니다. 이름을 확인해주세요.");
		return;
	}

	InitializeShaderResources();

	if (nullptr != Mesh)
	{
		CreateInputLayout();
	}
}

void URenderUnit::SetRenderingPipelineAndDraw(class UEngineCamera* _Camera, float _DeltaTime)
{
	for (std::pair<const EShaderType, UShaderBindingManager>& Pair : Resources)
	{
		Pair.second.BindAllShaderSlots();
	}

	Mesh->GetVertexBuffer()->IASetVertexBuffers();
	
	Mesh->GetIndexBuffer()->IASetIndexBuffer();
	Material->IASetPrimitiveTopology();
	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayout.Get());

	Material->GetVertexShader()->VSSetShader();

	Material->GetRasterizerState()->RSSetState();

	Material->GetPixelShader()->PSSetShader();

	Material->GetBlend()->OMSetBlendState();
	Material->GetDepthStencilState()->OMSetDepthStencilState();

	UEngineCore::GetDevice().GetContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderUnit::CreateInputLayout()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();

	UEngineInputLayoutInfo* InfoPtr = Mesh->GetVertexBuffer()->GetInputLayoutInfo();

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InfoPtr->InputLayOutData[0],
		static_cast<unsigned int>(InfoPtr->InputLayOutData.size()),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayout);

	int a = 0;
}

void URenderUnit::Reset()
{
	for (std::pair<const EShaderType, UShaderBindingManager>& Pair : Resources)
	{
		Pair.second.UnbindAllShaderSlots();
	}
}