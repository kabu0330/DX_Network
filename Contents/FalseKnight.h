#pragma once
#include "Monster.h"


// 설명 :
class AFalseKnight : public AMonster
{
public:
	// constrcuter destructer
	AFalseKnight();
	~AFalseKnight();

	// delete Function
	AFalseKnight(const AFalseKnight& _Other) = delete;
	AFalseKnight(AFalseKnight&& _Other) noexcept = delete;
	AFalseKnight& operator=(const AFalseKnight& _Other) = delete;
	AFalseKnight& operator=(AFalseKnight&& _Other) noexcept = delete;

	UFSMStateManager& GetFSM()
	{
		return FSM;
	}
	void AddHeadHp(int _Att)
	{
		HeadHp += _Att;
	}
	void DamageSound() override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

	void DamageLogic(int _KnightAtt) override;
	int PhaseHp = 75;
	int HeadHp = 40;

	int DeathCountValue = 3;

private:

	void BossPatternTimeElapsed(float _DeltaTime);
	float JumpElapsed = 0.0f;
	float JumpAttackElapsed = 0.0f;

	float JumpActionInitElapsed = 0.0f;
	bool bCanJumpAttack = false;
	bool bIsLandSound = false;
	
	bool bIsResting = false;
	float RestElapsed = 0.0f;


	void DamageEffect() override;

	bool bCanFlashEffect = false;
	bool bIsEndFlashEffect = false;
	void StartFlashEffect(float _DeltaTime);
	void EndFlashEffect(float _DeltaTime);

	void CreateAttackLogicAndEffect();
	bool bIsAttackEffect = false;
	void CreateGroundImpack();
	bool bIsOffsetAttack1Frame = false;
	bool bIsOffsetAttackRecovery1Fame = false;
	bool bIsDoubleAttack = false;

	void CreateJumpAttackLogicAndEffect();
	

	void CreateDropObject();
	bool bIsBerserkMode = false; // 첫 스턴 전까진 광폭 상태가 아니다.
	bool bCanBerserkAttack = true;
	float BerserkAttackElapsed = 0.0f;
	int DropObjectCount = 1;
	int BerserkAttackCount = 0;
	int BerserkAttackCountMax = 10;
	bool bIsBerserkVoice = false;

	void CreateHeadRenderer();
	class AFalseKnightHead* Head = nullptr;

	void ChangeStunAnimation();
	void CreateStunEffect();
	bool bIsStunEffect = false;
	bool bIsInit = true;
	bool bCanStunRecovery = false;
	bool bIsDeathAir = false;
	bool bIsStunVoice = false;
	bool bIsStunGroundSound = false;
	bool bIsStunRecoveryVocie = false;

	bool bIsDeath = false;

	void CreateDeathEffect(float _DeltaTime);
	void CreateDeathOrangeParticleEffect();
	float DeathEffectTimeElapesd = 0.0f;
	int DeathEffectCount = 1;

	void InitSpawn();
	bool bIsInitSpawn = true;

	void SetBarrier(bool _Value);

private:
	void SetIdle(float _DeltaTime) override;
	void SetJumpAnticipate(float _DeltaTime) override;
	void SetJump(float _DeltaTime) override;
	void SetLand(float _DeltaTime) override;

	void SetJumpAttackAnticipate(float _DeltaTime) override;
	void SetJumpAttack(float _DeltaTime) override;
	void SetJumpAttackRecovery(float _DeltaTime) override;
	void SetJumpAttackLand(float _DeltaTime) override;

	void SetAttackAnticipate(float _DeltaTime) override;
	void SetAttack(float _DeltaTime) override;
	void SetAttackRecovery(float _DeltaTime) override;
	void SetAttackRecovery2(float _DeltaTime) override;

	void SetBerserkAttackAnticipate(float _DeltaTime) override;
	void SetBerserkAttack(float _DeltaTime) override;
	void SetBerserkAttack2(float _DeltaTime) override;
	void SetBerserkAttackRecovery(float _DeltaTime) override;
	void SetBerserkAttackRecovery2(float _DeltaTime) override;

	void SetStun(float _DeltaTime) override;
	void SetStunOpen(float _DeltaTime) override;
	void SetStunHit(float _DeltaTime) override;
	void SetStunRecovery(float _DeltaTime) override;

	void SetDeathAir(float _DeltaTime) override;
	void SetDeathLand(float _DeltaTime) override;

	USoundPlayer PatternSound;
	USoundPlayer BossVoice;
	void BossPatternSound(std::string_view _SoundFile, float _Volume = 1.0f);
	bool bIsGushingSound = false;
	class APlayHUD* PlayHUD = nullptr;
};

