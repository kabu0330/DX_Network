#pragma once
#include "Monster.h"

// Ό³Έν :
class AWanderingHusk : public AMonster
{
public:
	// constrcuter destructer
	AWanderingHusk();
	~AWanderingHusk();

	// delete Function
	AWanderingHusk(const AWanderingHusk& _Other) = delete;
	AWanderingHusk(AWanderingHusk&& _Other) noexcept = delete;
	AWanderingHusk& operator=(const AWanderingHusk& _Other) = delete;
	AWanderingHusk& operator=(AWanderingHusk&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;
	void CreateCollision() override;

private:

};

