#pragma once
#include "Skill.h"
#include "KnightEffect.h"

// 설명 : 충돌체 + 이펙트
class AKnightSkill : public ASkill
{
public:
	// constrcuter destructer
	AKnightSkill();
	~AKnightSkill();

	// delete Function
	AKnightSkill(const AKnightSkill& _Other) = delete;
	AKnightSkill(AKnightSkill&& _Other) noexcept = delete;
	AKnightSkill& operator=(const AKnightSkill& _Other) = delete;
	AKnightSkill& operator=(AKnightSkill&& _Other) noexcept = delete;

	virtual void CreateHitEffect(class UCollision* _This, class UCollision* _Other);
	void CreateWhiteHitParticleEffect(class UCollision* _This, class UCollision* _Other);
	void CreateDamagedEffect(class UCollision* _This, class UCollision* _Other);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetCollisionEvent() override;
	void Collide(class UCollision* _This, class UCollision* _Other);

	virtual void Attack(class UCollision* _This, class UCollision* _Other);
	virtual void Knockback(class UCollision* _This, class UCollision* _Other);


	virtual void KnightKnockback(FVector _KnockbackDir);
	virtual void MonsterKnockback(class UCollision* _Other, FVector _KnockbackDir);

private:
	FVector KnightPos = { 0.0f, 0.0f, 0.0f };
	bool bIsParticle = false;
	inline static int Count = 0;
};

