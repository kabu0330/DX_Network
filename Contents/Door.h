#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ADoor : public AActor
{
public:
	// constrcuter destructer
	ADoor();
	~ADoor();

	// delete Function
	ADoor(const ADoor& _Other) = delete;
	ADoor(ADoor&& _Other) noexcept = delete;
	ADoor& operator=(const ADoor& _Other) = delete;
	ADoor& operator=(ADoor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetWarpPosition(class ARoom* _TargetRoom, FVector _TargetPos, bool _IsEnter);
	void SetScale(FVector _Scale);
	void Warp(class UCollision* _Actor1, class UCollision* _Actor2);
	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

	static bool IsDoorEnter()
	{
		return bIsDoorEnter;
	}
	static void SetDoorEnter(bool _Value)
	{
		bIsDoorEnter = _Value;
	}

protected:
	void ActiveKnightCollision();

private:
	inline static bool bIsDoorEnter = false;
	std::shared_ptr<class UCollision> BodyCollision;
	std::shared_ptr<class UTimeEventComponent> TimeEventer;
	FVector Scale;
	FVector TargetPos = FVector::ZERO;
	FVector InitPos = FVector::ZERO;
	class ARoom* TargetRoom = nullptr;
	bool bIsDoor = true;

	class AKnight* Knight = nullptr;
};

