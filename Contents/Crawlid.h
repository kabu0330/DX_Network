#pragma once
#include "Monster.h"

// 설명 :
class ACrawlid : public AMonster
{
public:
	// constrcuter destructer
	ACrawlid();
	~ACrawlid();

	// delete Function
	ACrawlid(const ACrawlid& _Other) = delete;
	ACrawlid(ACrawlid&& _Other) noexcept = delete;
	ACrawlid& operator=(const ACrawlid& _Other) = delete;
	ACrawlid& operator=(ACrawlid&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
	void SetAttackAnticipate(float _DeltaTime) override {} // 공격 안함
	void DamageEvent() override;
};

