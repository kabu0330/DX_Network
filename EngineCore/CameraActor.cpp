#include "PreCompile.h"
#include "CameraActor.h"
#include "EngineCamera.h"
#include <EnginePlatform/EngineInput.h>

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UEngineCamera>();
	RootComponent = CameraComponent;
}

ACameraActor::~ACameraActor()
{
	CameraComponent = nullptr;
}

void ACameraActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ACameraActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CameraComponent->CalculateViewAndProjection();

	// Free Camera
	ScreenPos = UEngineCore::GetMainWindow().GetMousePos();
	FVector ScreenDir = PrevScreenPos - ScreenPos;
	ScreenDir.Normalize();
	PrevScreenPos = ScreenPos;

	if (true == IsFreeCameraValue)
	{
		if (UEngineInput::IsDown('O'))
		{
			switch (CameraComponent->ProjectionType)
			{
			case EProjectionType::PERSPECTIVE:
				CameraComponent->ProjectionType = EProjectionType::ORTHOGRAPHIC;
				break;
			case EProjectionType::ORTHOGRAPHIC:
				CameraComponent->ProjectionType = EProjectionType::PERSPECTIVE;
				break;
			default:
				break;
			}
		}

		float Speed = FreeSpeed;

		if (UEngineInput::IsPress(VK_LSHIFT))
		{
			Speed *= 5.0f;
		}

		if (UEngineInput::IsPress('A'))
		{
			AddActorLocation(-GetActorTransform().GetWorldRight() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('D'))
		{
			AddActorLocation(GetActorTransform().GetWorldRight() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('W'))
		{
			AddActorLocation(GetActorTransform().GetWorldFoward() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('S'))
		{
			AddActorLocation(-GetActorTransform().GetWorldFoward() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('Q'))
		{
			AddActorLocation(GetActorTransform().GetWorldUp() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('E'))
		{
			AddActorLocation(-GetActorTransform().GetWorldUp() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress(VK_RBUTTON))
		{
			AddActorRotation({ -ScreenDir.Y * RotSpeed * _DeltaTime, -ScreenDir.X * RotSpeed * _DeltaTime });
		}
	}
}

FVector ACameraActor::ScreenPosToWorldPos(FVector _Pos)
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	_Pos = _Pos * ViewPort.InverseReturn();
	_Pos = _Pos * CameraTransform.Projection.InverseReturn();
	_Pos = _Pos * CameraTransform.View.InverseReturn();

	return _Pos;
}

FVector ACameraActor::WorldPosToScreenPos(FVector _Pos)
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	_Pos = _Pos * CameraTransform.View;
	_Pos = _Pos * CameraTransform.Projection;
	_Pos = _Pos * ViewPort;
	return _Pos;
}

FVector ACameraActor::ScreenMousePosToWorldPosWithOutPos()
{
	return FVector();
}

FVector ACameraActor::ScreenMousePosToWorldPos()
{
	//FVector Size = UEngineCore::GetMainWindow().GetWindowSize();
	//FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	//float4x4 Mat;
	//Mat.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	//FTransform CameraTransform = GetActorTransform();

	//MousePos = MousePos * Mat.InverseReturn();
	//MousePos = MousePos * CameraTransform.Projection.InverseReturn();
	//MousePos = MousePos * CameraTransform.View.InverseReturn();

	FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	return ScreenPosToWorldPos(MousePos);
	// 마우스 좌표가 100, 100
// 크기 이동 회전 공전 부모 * view * 투영 * viewport
}

void ACameraActor::FreeCameraOn()
{
	IsFreeCameraValue = true;
	FreeCameraCheck();
}

void ACameraActor::FreeCameraOff()
{
	IsFreeCameraValue = false;
}

void ACameraActor::FreeCameraSwitch()
{
	IsFreeCameraValue = !IsFreeCameraValue;
	FreeCameraCheck();
}

void ACameraActor::FreeCameraCheck()
{
	if (true == IsFreeCameraValue)
	{
		PrevTrans = GetActorTransform();
		PrevProjectionType = GetCameraComponent()->ProjectionType;
		GetCameraComponent()->ProjectionType = EProjectionType::PERSPECTIVE;
	}
	else
	{
		SetActorTransform(PrevTrans);
		GetCameraComponent()->ProjectionType = PrevProjectionType;
	}
}