#pragma once
#include <EngineCore/Actor.h>
#include <EnginePlatform/EngineSound.h>

// Ό³Έν :
class ABarrier : public AActor
{
public:
	// constrcuter destructer
	ABarrier();
	~ABarrier();

	// delete Function
	ABarrier(const ABarrier& _Other) = delete;
	ABarrier(ABarrier&& _Other) noexcept = delete;
	ABarrier& operator=(const ABarrier& _Other) = delete;
	ABarrier& operator=(ABarrier&& _Other) noexcept = delete;

	void SetActiveBarrier(bool _Value);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Barrier(class UCollision* _This, class UCollision* _Other);
	void BarrierEnd(class UCollision* _This, class UCollision* _Other);
private:
	class UContentsRenderer* Renderer = nullptr;
	class UCollision* Collision = nullptr;
	class UTimeEventComponent* TimeEventer = nullptr;
	USoundPlayer Sound;
	bool bIsDestory = false;
	float ZSort = 0.0f;
	FVector Scale = FVector::ZERO;
};

