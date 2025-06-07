#pragma once
#include "Actor.h"
#include "EngineEnums.h"

// ���� :
class ACameraActor : public AActor
{
public:
	// constrcuter destructer
	ENGINEAPI ACameraActor();
	ENGINEAPI ~ACameraActor();

	// delete Function
	ACameraActor(const ACameraActor& _Other) = delete;
	ACameraActor(ACameraActor&& _Other) noexcept = delete;
	ACameraActor& operator=(const ACameraActor& _Other) = delete;
	ACameraActor& operator=(ACameraActor&& _Other) noexcept = delete;

	ENGINEAPI std::shared_ptr<class UEngineCamera> GetCameraComponent()
	{
		return CameraComponent;
	}

	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void Tick(float _DeltaTime);

	ENGINEAPI FVector ScreenMousePosToWorldPos();

	// �׻� �߽��� 0,0 ���� ���� ���콺 ���� ��¹�
	ENGINEAPI FVector ScreenMousePosToWorldPosWithOutPos();


	ENGINEAPI FVector ScreenPosToWorldPos(FVector _Pos);

	ENGINEAPI FVector WorldPosToScreenPos(FVector _Pos);


	ENGINEAPI inline bool IsFreeCamera()
	{
		return IsFreeCameraValue;
	}

	ENGINEAPI void FreeCameraOn();

	ENGINEAPI void FreeCameraOff();

	ENGINEAPI void SwitchFreeCamera();

	void SetFreeCameraSpeed(float _Speed)
	{
		FreeSpeed = _Speed;
	}

protected:

private:
	std::shared_ptr<class UEngineCamera> CameraComponent = nullptr;

	bool IsFreeCameraValue = false;
	ENGINEAPI void CheckFreeCamera();

	FTransform PrevTrans;
	EProjectionType PrevProjectionType = EProjectionType::ORTHOGRAPHIC;
	float FreeSpeed = 2000.0f;

	FVector ScreenPos;
	FVector PrevScreenPos;
	float RotSpeed = 360.0f;
};

