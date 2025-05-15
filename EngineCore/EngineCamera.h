#pragma once
#include <map>
#include <list>
#include "Renderer.h"
#include "SceneComponent.h"
#include "EngineEnums.h"

// MinimapCamera;
// PlayerCamera
// PlayCamera
// UICamera

// 설명 : 뷰포트 설정, View / Projection 행렬 갱신, 렌더랑
class UEngineCamera : public USceneComponent
{
	friend class ULevel;
	friend class ACameraActor;

public:
	// constrcuter destructer
	ENGINEAPI UEngineCamera();
	ENGINEAPI ~UEngineCamera();

	// delete Function
	UEngineCamera(const UEngineCamera& _Other) = delete;
	UEngineCamera(UEngineCamera&& _Other) noexcept = delete;
	UEngineCamera& operator=(const UEngineCamera& _Other) = delete;
	UEngineCamera& operator=(UEngineCamera&& _Other) noexcept = delete;

	ENGINEAPI void BeginPlay() override;

	ENGINEAPI void Tick(float _DetlaTime);
	ENGINEAPI void Render(float _DetlaTime);
	ENGINEAPI void Release(float _DeltaTime);

	void CalculateViewAndProjection();

	ENGINEAPI void SetFar(float _Value)
	{
		Far = _Value;
	}

	ENGINEAPI void SetNear(float _Value)
	{
		Near = _Value;
	}

	ENGINEAPI void SetFOV(float _Value)
	{
		FOV = _Value;
	}

	ENGINEAPI void SetProjectionType(EProjectionType _Type)
	{
		ProjectionType = _Type;
	}

	ENGINEAPI void SetZSort(int _Order, bool _Value);

protected:


private:
	float Near = 1.0f;
	float Far = 5000.0f;
	float FOV = 60.0f;

	EProjectionType ProjectionType = EProjectionType::ORTHOGRAPHIC;

	D3D11_VIEWPORT ViewPortInfo = D3D11_VIEWPORT();

	FVector ProjectionScale = {0.0f, 0.0f};

	// 내가 바라보는 랜더러의 그룹은 카메라가 가진다.
	std::map<int, std::list<std::shared_ptr<class URenderer>>> Renderers;
	std::map<int, bool> RendererZSort;

	// 렌더 타겟을 카메라가 가진다.
	std::shared_ptr<class UEngineRenderTarget> CameraTarget = nullptr;
	// std::shared_ptr<class UEngineRenderTarget> LightTarget;
	// std::shared_ptr<class UEngineRenderTarget> ShadowTarget;

	void ChangeRenderGroup(int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer);
};

