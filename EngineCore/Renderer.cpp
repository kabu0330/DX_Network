#include "PreCompile.h"
#include "Renderer.h"
#include <EngineBase/EngineString.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/Mesh.h>
#include "EngineVertex.h"
#include "EngineBlend.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::SetOrder(int _Order)
{
	if (0 != GetOrder() && _Order == GetOrder())
	{
		return;
	}
	int PrevOrder = GetOrder();
	UObject::SetOrder(_Order);
	ULevel* Level = GetActor()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}

ENGINEAPI void URenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
	SetOrder(GetOrder());
}

URenderUnit& URenderer::GetRenderUnit(UINT _Index)
{
	return RenderUnits[_Index];
}

void URenderer::SetMesh(std::string_view _Name, UINT _Index /*= 0*/)
{
	URenderUnit& Unit = GetRenderUnit(_Index);
	Unit.SetMesh(_Name);
}

void URenderer::SetMaterial(std::string_view _Name, UINT _Index /*= 0*/)
{
	URenderUnit& Unit = GetRenderUnit(_Index);
	Unit.SetMaterial(_Name);
}

void URenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	this->CameraTransUpdate(_Camera); // WVP 연산

	for (size_t i = 0; i < RenderUnits.size(); i++)
	{
		RenderUnits[i].SetRenderingPipelineAndDraw(_Camera, _DeltaTime);
	}
}

URenderUnit& URenderer::CreateRenderUnit()
{
 	URenderUnit& NewUnit = RenderUnits.emplace_back(); // 내부에서 placement new를 통해 복사를 안해도 되게 최적화 해주는 문법
	NewUnit.ParentRenderer = this;
	return NewUnit;
}