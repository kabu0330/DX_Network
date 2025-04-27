#pragma once
#include "Monster.h"

// Ό³Έν :
class AHuskHornhead : public AMonster
{
public:
	// constrcuter destructer
	AHuskHornhead();
	~AHuskHornhead();

	// delete Function
	AHuskHornhead(const AHuskHornhead& _Other) = delete;
	AHuskHornhead(AHuskHornhead&& _Other) noexcept = delete;
	AHuskHornhead& operator=(const AHuskHornhead& _Other) = delete;
	AHuskHornhead& operator=(AHuskHornhead&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;
	void CreateCollision() override;

private:
	void CreateAttackLogicAndEffect() override;
};

