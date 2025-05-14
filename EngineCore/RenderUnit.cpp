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

void URenderUnit::CheckMaterialResources()
{
	if (nullptr == Material)
	{
		MSGASSERT("�������� �ʴ� ��Ƽ������ ���ҽ��� üũ�� �� �����ϴ�.");
		return;
	}

	{
		UEngineShaderResources& Vs = Material->GetVertexShader()->ShaderResources;
		Resources[EShaderType::VS] = Material->GetVertexShader()->ShaderResources;
	}

	{
		UEngineShaderResources& Ps = Material->GetPixelShader()->ShaderResources;
		Resources[EShaderType::PS] = Material->GetPixelShader()->ShaderResources;
	}

	// �θ� �������� ���ٸ�, �װ� UI��.
	if (nullptr != ParentRenderer)
	{
		TransformObject = ParentRenderer; // UI�� Ʈ������ ������Ʈ�� ������ �ִ�.
	}

	if (nullptr != TransformObject) // ��� ���̴��� ���鼭
	{
		for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
		{
			if (false == Resources.contains(i))
			{
				continue;
			}

			if (false == Resources[i].IsConstantBuffer("FTransform"))
			{
				continue;
			}

			FTransform& Ref = TransformObject->GetTransformRef();
			Resources[i].ConstantBufferLinkData("FTransform", Ref);
		}	
	}
}

void URenderUnit::ConstantBufferLinkData(std::string_view _Name, void* _Data)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsConstantBuffer(_Name))
		{
			continue;
		}

		Resources[i].ConstantBufferLinkData(_Name, _Data);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsTexture(_Name))
		{
			continue;
		}

		Resources[i].TextureSetting(_Name, _ResName);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsTexture(_Name))
		{
			continue;
		}

		Resources[i].TextureSetting(_Name, _Texture);
	}
}

void URenderUnit::SetSampler(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsSampler(_Name))
		{
			continue;
		}

		Resources[i].SamplerSetting(_Name, _ResName);
	}
}

void URenderUnit::SetMesh(std::string_view _Name)
{
	Mesh = UMesh::Find<UMesh>(_Name);

	if (nullptr == Mesh)
	{
		std::string Name = _Name.data();
		MSGASSERT("�������� �ʴ� �޽��Դϴ�." + Name);
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
		MSGASSERT("�������� �ʴ� ��Ƽ�������� �����Ϸ��� �߽��ϴ�." + Name);
	}

	CheckMaterialResources();

	if (nullptr != Mesh)
	{
		CreateInputLayout();
	}
}

void URenderUnit::Render(class UEngineCamera* _Camera, float _DeltaTime)
{
	for (std::pair<const EShaderType, UEngineShaderResources>& Pair : Resources)
	{
		Pair.second.Setting();
	}

	Mesh->GetVertexBuffer()->Setting();			//	InputAssembler1Setting();
	Material->GetVertexShader()->Setting();		//	VertexShaderSetting();
	
	Mesh->GetIndexBuffer()->Setting();			//	InputAssembler2Setting();
	Material->PrimitiveTopologySetting();

	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayout.Get());

	Material->GetRasterizerState()->Setting(); 	//	RasterizerSetting();

	Material->GetPixelShader()->Setting(); 	//	PixelShaderSetting();

	//	OutPutMergeSetting();
	// ����Ÿ���� �ٲ��.
	Material->GetBlend()->Setting();
	Material->GetDepthStencilState()->Setting();

	UEngineCore::GetDevice().GetContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderUnit::CreateInputLayout()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();

	UEngineInputLayoutInfo* InfoPtr = Mesh->GetVertexBuffer()->GetInfoPtr();

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
	for (std::pair<const EShaderType, UEngineShaderResources>& Pair : Resources)
	{
		Pair.second.Reset();
	}
}