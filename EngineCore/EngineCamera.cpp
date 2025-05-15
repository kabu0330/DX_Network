#include "PreCompile.h"
#include "EngineCamera.h"
#include "Renderer.h"
#include "EngineRenderTarget.h"

UEngineCamera::UEngineCamera()
{
}

void UEngineCamera::BeginPlay()
{
	USceneComponent::BeginPlay();

	FVector Scale = UEngineCore::GetScreenScale();

	ProjectionScale = Scale;

	// 뷰포트 세팅을 여기서 한다.
	ViewPortInfo.Width = UEngineCore::GetScreenScale().X;
	ViewPortInfo.Height = UEngineCore::GetScreenScale().Y;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;

	// 카메라마다 렌더 타겟을 가지게 된다.
	CameraTarget = std::make_shared<UEngineRenderTarget>();
	CameraTarget->CreateRenderTargetView(UEngineCore::GetScreenScale());
	CameraTarget->CreateDepthTexture();
}

UEngineCamera::~UEngineCamera()
{
	Renderers.clear();
}

void UEngineCamera::Tick(float _DetlaTime)
{
	// 카메라는 Tick에서 View와 Projection을 계산 후 렌더러로 전달
	Transform.View;
	Transform.Projection;
	int a = 0;
}

// 카메라에서 렌더링한다.
void UEngineCamera::Render(float _DetlaTime)
{
	// 랜더링 진입하기 전에 한번 뷰포트 세팅하고 
	UEngineCore::GetDevice().GetContext()->RSSetViewports(1, &ViewPortInfo);

	CameraTarget->ClearRenderTargetView();	// 화면을 지우고
	CameraTarget->OMSetRenderTargets(); // 출력 병합한다.

	// Ranged for를 돌릴때는 복사가 일어난다.
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;

		if (true == RendererZSort[RenderGroup.first]) // Z Sort
		{
			// 둘의 z값이 완전히 겹쳐있을때는 답이 없다.
			// 크기 비교해서 크기가 더 작은쪽을 왼쪽으로 옮긴다.
			RenderList.sort([](std::shared_ptr<URenderer>& _Left, std::shared_ptr<URenderer>& _Right)
				{
					return _Left->GetTransformRef().WorldLocation.Z > _Right->GetTransformRef().WorldLocation.Z;
				});
		}

		for (std::shared_ptr<URenderer> Renderer : RenderList)
		{
			if (false == Renderer->IsActive()) // 렌더러가 비활성화 상태면 무시하고
			{
				continue;
			}

			Renderer->Render(this, _DetlaTime);
		}
	}
}

void UEngineCamera::Release(float _DeltaTime)
{
	// Ranged for를 돌릴때는 복사가 일어나므로
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;
		std::list<std::shared_ptr<URenderer>>::iterator StartIter = RenderList.begin();
		std::list<std::shared_ptr<URenderer>>::iterator EndIter = RenderList.end();

		// 언리얼은 중간에 삭제할수 없어.
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy()) // Destory가 false면 삭제하지 않는다.
			{
				++StartIter;
				continue;
			}

			// 컴포넌트의 메모리를 삭제할수 권한은 오로지 액터만 가지고 있다.
			// 그저 RenderList에서 제거할 뿐.
			StartIter = RenderList.erase(StartIter);
		}
	}
}

void UEngineCamera::SetZSort(int _Order, bool _Value)
{
	RendererZSort[_Order] = _Value;
}

void UEngineCamera::ChangeRenderGroup(int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_PrevGroupOrder].remove(_Renderer); // 이전 렌더 그룹에서 삭제하고
	Renderers[_Renderer->GetOrder()].push_back(_Renderer); // 새로운 렌더 그룹에 넣는다.
}

void UEngineCamera::CalculateViewAndProjection()
{
	FTransform& Trans = GetTransformRef();

	Trans.View.View(Trans.World.ArrVector[3], Trans.World.GetFoward(), Trans.World.GetUp());

	switch (ProjectionType)
	{
	case EProjectionType::PERSPECTIVE:
		Trans.Projection.PerspectiveFovDeg(FOV, ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	case EProjectionType::ORTHOGRAPHIC:
		Trans.Projection.OrthographicLH(ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	default:
		break;
	}
}