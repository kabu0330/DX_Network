#pragma once
#include "KnightEffect.h"
#include <EngineCore/TimeEventComponent.h>

// Ό³Έν :
class KnightStunEffect : public AKnightEffect
{
public:
	// constrcuter destructer
	KnightStunEffect();
	~KnightStunEffect();

	// delete Function
	KnightStunEffect(const KnightStunEffect& _Other) = delete;
	KnightStunEffect(KnightStunEffect&& _Other) noexcept = delete;
	KnightStunEffect& operator=(const KnightStunEffect& _Other) = delete;
	KnightStunEffect& operator=(KnightStunEffect&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CreateStunImpactEffect();

protected:

private:
	UTimeEventComponent* TimeEventer = nullptr;
};

