#pragma once
#include <EngineCore/Actor.h>
#include "ContentRenderer.h"

// 설명 :
class AEffect : public AActor
{
public:
	AEffect();
	~AEffect();

	// delete Function
	AEffect(const AEffect& _Other) = delete;
	AEffect(AEffect&& _Other) noexcept = delete;
	AEffect& operator=(const AEffect& _Other) = delete;
	AEffect& operator=(AEffect&& _Other) noexcept = delete;

	class UContentRenderer* GetRenderer()
	{
		return BodyRenderer.get();
	}
	
	void ChangeAnimation(std::string_view _AnimationName);
	void ChangeAnimation(std::string_view _AnimationName, FVector _Pos)
	{
		ChangeAnimation(_AnimationName);
		SrcPos = _Pos;
	}
	virtual void EndFunction() {} // 렌더러 디스트로이 직전 함수 호출


	virtual void Release();

	void SetZSort(int _Value);
	void SetZSort(EZOrder _Value)
	{
		SetZSort(static_cast<int>(_Value));
	}
	float GetZSort()
	{
		return BodyRenderer->GetWorldLocation().Z;
	}

	void SetLocation(AActor* _Actor, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		TargetActor = _Actor;
		Offset = _OffsetPos;
		Rotation = _Rotation;

		SrcPos = TargetActor->GetActorLocation();
		SetActorLocation(SrcPos);
		CheckDirection();
	}

	// 위치를 실시간으로 공유하면서 데리고 다니고 싶다면, 몬스터는 사용 비권장
	void SetTargetActor(AActor* _TargetActor)
	{
		TargetActor = _TargetActor;
	}

	void SetLocation(FVector _Pos)
	{
		SrcPos = _Pos;
	}

	void AddLocation(AActor* _Actor, FVector _Speed, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		bIsAddLocation = true;
		Speed = _Speed;
		SetLocation(_Actor, _OffsetPos, _Rotation);
	}

	// 좌우 반전
	void ToggleFlip()
	{
		RotationScale = { -1.0f, 1.0f, 1.0f };
	}
	void SwitchRotationScale()
	{
		RotationScale.X *= -1.0f;
	}

	// 좌우 반전을 원치 않는다면 false
	void EnableRotation(bool _bIsRotation)
	{
		bIsRotation = _bIsRotation;
	}
	void SetScale(float _Ratio)
	{
		ScaleRatio = _Ratio;
		BodyRenderer->SetAutoScaleRatio(_Ratio);
	}
	void SetScaleFadeOut(float _ReductionRate)
	{
		bIsAutoRelease = false; // 애니메이션 시간이 아닌 크기 비율이 0이하가 되면 소멸
		bIsScaleDecay = true;
		ScaleReductionRate = _ReductionRate;
	}
	void SetAlphaFadeOut(float _ReductionRate, float _MinAlpha = 0.0f)
	{
		bIsAutoRelease = false; // 알파가 0이 되면 소멸
		bIsAlphaDecay = true;
		DeltaAlpha = _ReductionRate;
		MinAlpha = _MinAlpha;
	}
	void SetAlphaFadeInFadeOut(float _DeltaAlpha, float _MaxAlpha = 1.0f)
	{
		bIsAutoRelease = false; // 맥스 알파가 되면 소멸
		bCanIncreaseAlpha = true;
		DeltaAlpha = _DeltaAlpha;
		MaxAlpha = _MaxAlpha;
	}

	void SetAutoRelease(bool _bIsAutoRelease)
	{
		bIsAutoRelease = _bIsAutoRelease;
	}

	bool bIsValid = true;

	bool IsPixelCollision()
	{
		return bIsPixelCollision;
	}

protected:
	class UTimeEventComponent* TimeEventer = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetPosition();
	void CheckDirection();

	AActor* TargetActor = nullptr;
	FVector SrcPos = FVector::ZERO;
	FVector Offset = FVector::ZERO;
	FVector Rotation = FVector::ZERO;
	FVector Speed = FVector::ZERO;
	FVector CurPos = FVector::ZERO;
	FVector RotationScale = { 1.0f, 1.0f, 1.0f };
	bool bIsAddLocation = false;
	
	bool bIsLeft = true;
	bool bIsRotation = true;
	bool bIsAutoRelease = true;
	bool bIsSwitchDirection = true;
	bool bInitSetPos = false;

	float ZSort = 0.0f;
	std::shared_ptr<UContentRenderer> BodyRenderer;
private:

	float ScaleRatio = 0.0f; // 파티클 옵션
	bool bIsScaleDecay = false;
	float ScaleReductionRate = 0.0f;
	void SetScaleDecay(float _DeltaTime);

	bool bIsAlphaDecay = false;
	float DeltaAlpha = 0.0f;
	float MinAlpha = 0.0f;
	void SetAlphaDecay(float _DeltaTime);

	bool bCanIncreaseAlpha = false;
	float MaxAlpha = 1.0f;
	void IncreaseAlpha(float _DeltaTime);

public:
	// 픽셀 충돌

	// 픽셀충돌 적용 필수 함수 : 부모를 알아야 픽셀 이미지와 충돌 검사를 할 수 있다.
	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}

protected:
	// 픽셀 충돌
	bool bIsPixelCollision = false;
	class ARoom* ParentRoom = nullptr;
};

