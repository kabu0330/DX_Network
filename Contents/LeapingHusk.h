#pragma once
#include "Monster.h"

// Ό³Έν :
class ALeapingHusk : public AMonster
{
public:
	// constrcuter destructer
	ALeapingHusk();
	~ALeapingHusk();

	// delete Function
	ALeapingHusk(const ALeapingHusk& _Other) = delete;
	ALeapingHusk(ALeapingHusk&& _Other) noexcept = delete;
	ALeapingHusk& operator=(const ALeapingHusk& _Other) = delete;
	ALeapingHusk& operator=(ALeapingHusk&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;
	void CreateCollision() override;

private:
	void SetAttack(float _DeltaTime) override;
	void CreateAttackLogicAndEffect() override;
};

