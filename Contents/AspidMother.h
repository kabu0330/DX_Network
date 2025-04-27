#pragma once
#include "Monster.h"

// Ό³Έν :
class AAspidMother : public AMonster
{
public:
	// constrcuter destructer
	AAspidMother();
	~AAspidMother();

	// delete Function
	AAspidMother(const AAspidMother& _Other) = delete;
	AAspidMother(AAspidMother&& _Other) noexcept = delete;
	AAspidMother& operator=(const AAspidMother& _Other) = delete;
	AAspidMother& operator=(AAspidMother&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
	void SetAttack(float _DeltaTime) override;
	void SpawnAspidHatchling();
	void DeathPattern();

	bool bIsSpawn = false;
	float ChildZSort = 0.0f;
};

