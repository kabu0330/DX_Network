#pragma once
#include "Monster.h"

// 설명 :
class AGruzzer : public AMonster
{
public:
	// constrcuter destructer
	AGruzzer();
	~AGruzzer();

	// delete Function
	AGruzzer(const AGruzzer& _Other) = delete;
	AGruzzer(AGruzzer&& _Other) noexcept = delete;
	AGruzzer& operator=(const AGruzzer& _Other) = delete;
	AGruzzer& operator=(AGruzzer&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
	void SetAttackAnticipate(float _DeltaTime) override {} // 공격 안함
};

