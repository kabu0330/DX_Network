#include "PreCompile.h"
#include "TestPostEffect.h"

UTestPostEffect::UTestPostEffect()
{
}

UTestPostEffect::~UTestPostEffect()
{
}

void UTestPostEffect::Init()
{
	UPostEffect::Init();

	CopyTarget = std::make_shared<UEngineRenderTarget>();
	CopyTarget->CreateRenderTargetView(UEngineCore::GetScreenScale());

	RenderUnit.SetMesh("FullRect");
	RenderUnit.SetMaterial("JHJEffect");
}

void UTestPostEffect::Effect(UEngineCamera* _Camera, float _DeltaTime)
{
	UPostEffect::Effect(_Camera, _DeltaTime);

	ResultTarget->CopyTo(CopyTarget);
	ResultTarget->OMSetRenderTargets();

	RenderUnit.BindTextureToShaderSlot("ImageTexture", CopyTarget->GetTexture());
	RenderUnit.SetRenderingPipelineAndDraw(_Camera, _DeltaTime);
	RenderUnit.Reset();

}

