#pragma once
#include "EngineShaderResources.h"
#include "Mesh.h"
#include "EngineMaterial.h"
#include "EngineEnums.h"

// 설명 : 랜더링의 최소단위
// 주인공이라면 
// 주인공의 머리  RenderUnit
// 주인공의 몸  RenderUnit
// 주인공의 팔  RenderUnit
// 주인공의 갑옷 RenderUnit
class URenderUnit
{
public:
	// constrcuter destructer
	URenderUnit();
	~URenderUnit();

	// 이제 SceneComponent가 아닌 TransformObject가 크기, 회전, 위치값을 가진다. UI는 액터의 컴포넌트가 될 수 없는 구조로 들어와서 <- 언리얼식
	UTransformObject* TransformObject = nullptr;

	class URenderer* ParentRenderer = nullptr;

	// 매쉬(육체) 
	std::shared_ptr<UMesh> Mesh;
	// 머티리얼(피부)
	std::shared_ptr<UEngineMaterial> Material;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayOut;

	ENGINEAPI void SetMesh(std::string_view _Name);
	ENGINEAPI void SetMaterial(std::string_view _Name);

	ENGINEAPI virtual void Render(class UEngineCamera* _Camera, float _DeltaTime);

	ENGINEAPI void MaterialResourcesCheck();

	template<typename Data>
	ENGINEAPI void ConstantBufferLinkData(std::string_view _Name, Data& _Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&_Data));
	}

	ENGINEAPI void ConstantBufferLinkData(std::string_view Name, void* _Data);

	ENGINEAPI void SetTexture(std::string_view _Name, std::string_view _ResName);
	ENGINEAPI void SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);
	ENGINEAPI void SetSampler(std::string_view Name, std::string_view _ResName);

	//ENGINEAPI void SetTexture(std::string_view _Name, UEngineTexture* _Texture);

	std::map<EShaderType, UEngineShaderResources> Resources;

	ENGINEAPI void Reset();

private:

	void InputLayOutCreate();
};

