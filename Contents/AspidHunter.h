#pragma once
#include "Monster.h"

// Ό³Έν :
class AAspidHunter : public AMonster
{
public:
	// constrcuter destructer
	AAspidHunter();
	~AAspidHunter();

	// delete Function
	AAspidHunter(const AAspidHunter& _Other) = delete;
	AAspidHunter(AAspidHunter&& _Other) noexcept = delete;
	AAspidHunter& operator=(const AAspidHunter& _Other) = delete;
	AAspidHunter& operator=(AAspidHunter&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
	void SetAttack(float _DeltaTime) override;
	void CreateAttackLogicAndEffect() override;
	bool bIsFire = false;
};

