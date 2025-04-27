#pragma once
#include "Monster.h"

// Ό³Έν :
class AAspidHatchling : public AMonster
{
public:
	// constrcuter destructer
	AAspidHatchling();
	~AAspidHatchling();

	// delete Function
	AAspidHatchling(const AAspidHatchling& _Other) = delete;
	AAspidHatchling(AAspidHatchling&& _Other) noexcept = delete;
	AAspidHatchling& operator=(const AAspidHatchling& _Other) = delete;
	AAspidHatchling& operator=(AAspidHatchling&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
};

