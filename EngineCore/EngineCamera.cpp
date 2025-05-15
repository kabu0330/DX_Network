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

	// ����Ʈ ������ ���⼭ �Ѵ�.
	ViewPortInfo.Width = UEngineCore::GetScreenScale().X;
	ViewPortInfo.Height = UEngineCore::GetScreenScale().Y;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;

	// ī�޶󸶴� ���� Ÿ���� ������ �ȴ�.
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
	// ī�޶�� Tick���� View�� Projection�� ��� �� �������� ����
	Transform.View;
	Transform.Projection;
	int a = 0;
}

// ī�޶󿡼� �������Ѵ�.
void UEngineCamera::Render(float _DetlaTime)
{
	// ������ �����ϱ� ���� �ѹ� ����Ʈ �����ϰ� 
	UEngineCore::GetDevice().GetContext()->RSSetViewports(1, &ViewPortInfo);

	CameraTarget->ClearRenderTargetView();	// ȭ���� �����
	CameraTarget->OMSetRenderTargets(); // ��� �����Ѵ�.

	// Ranged for�� �������� ���簡 �Ͼ��.
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;

		if (true == RendererZSort[RenderGroup.first]) // Z Sort
		{
			// ���� z���� ������ ������������ ���� ����.
			// ũ�� ���ؼ� ũ�Ⱑ �� �������� �������� �ű��.
			RenderList.sort([](std::shared_ptr<URenderer>& _Left, std::shared_ptr<URenderer>& _Right)
				{
					return _Left->GetTransformRef().WorldLocation.Z > _Right->GetTransformRef().WorldLocation.Z;
				});
		}

		for (std::shared_ptr<URenderer> Renderer : RenderList)
		{
			if (false == Renderer->IsActive()) // �������� ��Ȱ��ȭ ���¸� �����ϰ�
			{
				continue;
			}

			Renderer->Render(this, _DetlaTime);
		}
	}
}

void UEngineCamera::Release(float _DeltaTime)
{
	// Ranged for�� �������� ���簡 �Ͼ�Ƿ�
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;
		std::list<std::shared_ptr<URenderer>>::iterator StartIter = RenderList.begin();
		std::list<std::shared_ptr<URenderer>>::iterator EndIter = RenderList.end();

		// �𸮾��� �߰��� �����Ҽ� ����.
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy()) // Destory�� false�� �������� �ʴ´�.
			{
				++StartIter;
				continue;
			}

			// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
			// ���� RenderList���� ������ ��.
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
	Renderers[_PrevGroupOrder].remove(_Renderer); // ���� ���� �׷쿡�� �����ϰ�
	Renderers[_Renderer->GetOrder()].push_back(_Renderer); // ���ο� ���� �׷쿡 �ִ´�.
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