#pragma once
#include "ShaderBindingManager.h"
#include "Mesh.h"
#include "EngineMaterial.h"
#include "EngineEnums.h"
#include "EngineDataStruct.h"

// ���� : �������� �ּҴ���
// ���ΰ��̶�� 
// ���ΰ��� �Ӹ�  RenderUnit
// ���ΰ��� ��  RenderUnit
// ���ΰ��� ��  RenderUnit
// ���ΰ��� ���� RenderUnit
class URenderUnit
{
public:
	ENGINEAPI URenderUnit();
	ENGINEAPI ~URenderUnit();

	UTransformObject* TransformObject = nullptr;

	class URenderer* ParentRenderer = nullptr;

	std::shared_ptr<UMesh> Mesh; 				// �Ž�(��ü) 
	std::shared_ptr<UEngineMaterial> Material; 	// ��Ƽ����(�Ǻ�)

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

	FRenderBaseData RenderBaseData;

	ENGINEAPI void SetMesh(std::string_view _Name);
	ENGINEAPI void SetMaterial(std::string_view _Name);

	ENGINEAPI virtual void SetRenderingPipelineAndDraw(class UEngineCamera* _Camera, float _DeltaTime);

	ENGINEAPI void InitializeShaderResources();

	template<typename Data>
	ENGINEAPI void ConstantBufferLinkData(std::string_view _Name, Data& _Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&_Data));
	}

	ENGINEAPI void ConstantBufferLinkData(std::string_view Name, void* _Data);

	ENGINEAPI void BindTextureToShaderSlot(std::string_view _Name, std::string_view _ResName);
	ENGINEAPI void BindTextureToShaderSlot(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);
	ENGINEAPI void BindTextureToShaderSlot(std::string_view _Name, UEngineTexture* _Texture);

	ENGINEAPI void SetSampler(std::string_view Name, std::string_view _ResName);

	std::map<EShaderType, UShaderBindingManager> Resources;

	ENGINEAPI void Reset();

private:
	void CreateInputLayout();

};

