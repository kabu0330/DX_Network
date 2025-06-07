#pragma once
#include <EngineCore/Actor.h>
#include "ContentRenderer.h"

// ���� :
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
	virtual void EndFunction() {} // ������ ��Ʈ���� ���� �Լ� ȣ��


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

	// ��ġ�� �ǽð����� �����ϸ鼭 ������ �ٴϰ� �ʹٸ�, ���ʹ� ��� �����
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

	// �¿� ����
	void ToggleFlip()
	{
		RotationScale = { -1.0f, 1.0f, 1.0f };
	}
	void SwitchRotationScale()
	{
		RotationScale.X *= -1.0f;
	}

	// �¿� ������ ��ġ �ʴ´ٸ� false
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
		bIsAutoRelease = false; // �ִϸ��̼� �ð��� �ƴ� ũ�� ������ 0���ϰ� �Ǹ� �Ҹ�
		bIsScaleDecay = true;
		ScaleReductionRate = _ReductionRate;
	}
	void SetAlphaFadeOut(float _ReductionRate, float _MinAlpha = 0.0f)
	{
		bIsAutoRelease = false; // ���İ� 0�� �Ǹ� �Ҹ�
		bIsAlphaDecay = true;
		DeltaAlpha = _ReductionRate;
		MinAlpha = _MinAlpha;
	}
	void SetAlphaFadeInFadeOut(float _DeltaAlpha, float _MaxAlpha = 1.0f)
	{
		bIsAutoRelease = false; // �ƽ� ���İ� �Ǹ� �Ҹ�
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

	float ScaleRatio = 0.0f; // ��ƼŬ �ɼ�
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
	// �ȼ� �浹

	// �ȼ��浹 ���� �ʼ� �Լ� : �θ� �˾ƾ� �ȼ� �̹����� �浹 �˻縦 �� �� �ִ�.
	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}

protected:
	// �ȼ� �浹
	bool bIsPixelCollision = false;
	class ARoom* ParentRoom = nullptr;
};

