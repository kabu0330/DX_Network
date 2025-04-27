#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/Actor.h>
#include "Global.h"
#include "StatusUnit.h"
#include <EngineCore/TimeEventComponent.h>
#include "MonsterSkill.h"

// 설명 :
class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster() {};

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	bool IsPause();
	void SetPause();
	void SwitchDebugPause()
	{
		bIsDebugPause = !bIsDebugPause;
	}

	bool IsLeft()
	{
		return bIsLeft;
	}

	bool IsKnockback()
	{
		return bIsKnockback;
	}

	bool IsFlip()
	{
		return bIsFlip;
	}

	virtual void DamageEvent() {}

	UContentsRenderer* GetRenderer() const
	{
		return BodyRenderer.get();
	}

	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

	UCollision* GetPixelCollision()
	{
		return PixelCollision.get();
	}

	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}
	class ARoom* GetParentRoom()
	{
		return ParentRoom;
	}

	UStatusUnit& GetStatRef()
	{
		return Stat;
	}
	UFSMStateManager& GetFSMRef()
	{
		return FSM;
	}

	bool CanFly()
	{
		return bCanFly;
	}

	// 몬스터 동작
	bool IsPlayerNearby();
	FVector GetDirectionToPlayer();
	void CheckDirectionToPlayer();
	FVector GetRandomDirection();

	// 히트 이펙트
	virtual void DamageEffect();
	virtual void CreateOrangeParticleEffect();
	void CreateHitOrangeEffect();

	virtual void DamageLogic(int _KnightAtt) {};

	virtual void DamageSound() {};

	void SetBarrier(bool _bIsBarrier)
	{
		bIsBarrier = _bIsBarrier;
	}
	void GetBarrierPos(FVector _BarrierPos)
	{
		BarrierPos = _BarrierPos;
	}

protected:
	bool bIsBarrier = false;
	FVector BarrierPos = FVector::ZERO;


	// 반드시 자식 클래스가 재정의 해야하는 가상함수
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	virtual void SetStatus();
	virtual void SetOffset();
	virtual void CreateAnimation();

	virtual void CreateCollision();

	virtual void CreateAttackLogicAndEffect();
	bool bIsShowEffect = false;

	void ResetRendererOffset();
	void ResetRendererOffset(UContentsRenderer* _Renderer);
	void SetRendererOffset(FVector _Offset);
	void SetRendererOffset(UContentsRenderer* _Renderer, FVector _Offset);
	void SetCollisionOffset(UCollision* _Target, FVector _Offset);

	void SetWalkRendererOffset();
	FVector WalkRendererOffset = FVector::ZERO;

	void SetAttackRendererOffset();
	FVector AttackRendererOffset = FVector::ZERO;

	void CreateCenterPoint();

	void CreatePixelCollision();

	void CheckDeath();
	void ReverseForce(float _DeltaTime);
	void Knockback(float _DeltaTime);
	void DeathAir(float _DeltaTime);
	void Death(float _DeltaTime);

	// Idle
	bool bIsFirstIdle = true;

	// 이동
	void Move(float _DeltaTime);
	bool bChooseDirection = false; // 이동할 방향을 결심했으면 이동하면서 방향전환 하지마.
	bool bCanMove = true;
	float MoveDuration = 2.0f;
	float MoveCooldown = 3.0f;
	float MoveElapsed = 0.0f;

	// 쿨타임
	void TimeElapsed(float _DeltaTime);
	void ResetAttackCooldown();

	// 동작
	bool bIsAggressive = true; // 플레이어 탐색하냐
	bool bCanJump = false;
	bool bCanRotation = true; // 지금 방향 전환 가능한 타이밍인가
	bool bIsPrevLeft = true;
	bool bIsTurn = false;
	bool bIsChasing = false; // 탐색 충돌체로 true/false 결정

	// 추적
	bool bIsChangeChasingDir = false;
	float ChasingDirectonCooldown = 0.5f;
	float ChasingDirectionElapsed = 0.0f;

	// 전투
	virtual void Dash();
	void Jump(float _DeltaTime);
	bool bCanAttack = true;
	float AttackDuration = 0.2f; // 공격 지속시간.
	float AttackCooldown = 2.0f;
	float AttackElapsed = 0.0f;
	float AttackFrameElapsed = 0.0f;
	bool bIsStun = false;

	// 기타 스텟
	float HitStunDuration = 1.0f;
	float JumpForce = 0.0f;
	float InitJumpForce = 600.0f;
	float JumpForceMax = 1800.0f;

	// 사망
	float DeathSpriteOffset = -20.0f;
	float DeathAirDistance = 400.0f;
	bool bIsDeathDestroy = false; // 죽으면 디스트로이 할거냐
	virtual void DeathPattern() {}

	UStatusUnit Stat = UStatusUnit();
	UTimeEventComponent* TimeEventer = nullptr;
	class AKnight* Knight = nullptr;

	// 충돌관련
	void SetCollisionEvent();
	void OnBodyCollision(class UCollision* _This, class UCollision* _Other);
	void Knockback();
	bool bIsKnockback = false;

	void DebugInput(float _DeltaTime);

	// 공격 또는 피격 동작 중일 때


	bool bIsPause = false; // 나이트가 몬스터가 존재하는 위치와 다르면
	bool bIsDebugPause = false;



	class ARoom* ParentRoom = nullptr;
	class ARoom* CurRoom = nullptr; 


	void CreateDetectCollision();
	FVector RendererOffset = FVector::ZERO;
	FVector BodyCollisionOffset = FVector::ZERO;
	FVector InitBodyCollisionOffset = FVector::ZERO;
	FVector DetectRange = FVector::ZERO;
	FVector CollisionScale = FVector::ZERO;
	float DeathRotation = 0.0f;
	float ZSort = 0.0f;
	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	std::shared_ptr<class UCollision> BodyCollision;
	std::shared_ptr<class UCollision> CenterPoint;
	std::shared_ptr<class UCollision> PixelCollision;
	std::shared_ptr<class UCollision> DetectCollision;

	void ChangeNextState(EMonsterState _NextState);
	void ChangePrevState();

protected:
	bool IsCurRoom();
	virtual bool CanAction();
	bool CanJump();

	using StateCallback = void(AMonster::*)(float);
	void CreateState(EMonsterState _State, StateCallback _Callback, std::string_view _AnimationName);
	
	void SetFSM();
	UFSMStateManager FSM;
	EMonsterState NextState = EMonsterState::IDLE;

	// 이동
	void UpdateFSM(float _DeltaTime);
	virtual void SetIdle(float _DeltaTime);
	virtual void SetWalkAnticipate(float _DeltaTime);
	virtual void SetWalk(float _DeltaTime);
	virtual void SetRun(float _DeltaTime);
	virtual void SetTurn(float _DeltaTime);

	// 대시
	virtual void SetDashArrive(float _DeltaTime);
	virtual void SetDashAnticipate(float _DeltaTime);
	virtual void SetDash(float _DeltaTime);
	virtual void SetDashAttack(float _DeltaTime);
	virtual void SetDashRecovery(float _DeltaTime);

	// 점프
	virtual void SetJumpAnticipate(float _DeltaTime);
	virtual void SetJump(float _DeltaTime);
	virtual void SetJumping(float _DeltaTime) {}
	virtual void SetLand(float _DeltaTime);
	virtual void SetHardLand(float _DeltaTime);

	virtual void SetJumpAttackAnticipate(float _DeltaTime);
	virtual void SetJumpAttack(float _DeltaTime);
	virtual void SetJumpAttackRecovery(float _DeltaTime);
	virtual void SetJumpAttackLand(float _DeltaTime) {}

	// 공격
	virtual void SetAttackAnticipate(float _DeltaTime);
	virtual void SetAttack(float _DeltaTime);
	virtual void SetAttackRecovery(float _DeltaTime);
	virtual void SetAttackRecovery2(float _DeltaTime) {}

	virtual void SetBerserkAttackAnticipate(float _DeltaTime) {}
	virtual void SetBerserkAttack(float _DeltaTime) {}
	virtual void SetBerserkAttack2(float _DeltaTime) {}
	virtual void SetBerserkAttackRecovery(float _DeltaTime) {}
	virtual void SetBerserkAttackRecovery2(float _DeltaTime) {}

	virtual void SetThrowAnticipate(float _DeltaTime);
	virtual void SetThrow(float _DeltaTime);
	virtual void SetThrowRecovery(float _DeltaTime);

	virtual void SetWallAnticipate(float _DeltaTime);
	virtual void SetWall(float _DeltaTime);
	virtual void SetWallAttack(float _DeltaTime);
	virtual void SetWallRecovery(float _DeltaTime);

	virtual void SetCounterAnticipate(float _DeltaTime);
	virtual void SetCounterStance(float _DeltaTime);
	virtual void SetCounter(float _DeltaTime);
	virtual void SetCounterAttack(float _DeltaTime);
	virtual void SetCounterAttackRecovery(float _DeltaTime);
	virtual void SetCounterRecovery(float _DeltaTime);

	// 방어
	virtual void SetShieldAnticipate(float _DeltaTime);
	virtual void SetShieldFront(float _DeltaTime);
	virtual void SetShieldFrontBump(float _DeltaTime);
	virtual void SetShieldTop(float _DeltaTime);
	virtual void SetShieldTopBump(float _DeltaTime);

	// 회피
	virtual void SetEvadeAnticipate(float _DeltaTime);
	virtual void SetEvade(float _DeltaTime);

	// 피격
	virtual void SetHit(float _DeltaTime);
	virtual void SetStun(float _DeltaTime);
	virtual void SetStunRoll(float _DeltaTime);
	virtual void SetStunRollEnd(float _DeltaTime);
	virtual void SetStunOpen(float _DeltaTime);
	virtual void SetStunHit(float _DeltaTime);
	virtual void SetStunRecovery(float _DeltaTime) {}

	// 사망
	virtual void SetDeathAir(float _DeltaTime);
	virtual void SetDeathLand(float _DeltaTime);

	// 공중
	virtual void SetFly(float _DeltaTime);
	virtual void SetFire(float _DeltaTime);
	virtual void SetBurst(float _DeltaTime);

	float AttackFrame = 0.1f;
	float AttackRecoveryFrame = 0.1f;

	void CheckDirection();
	bool bIsLeft = true;
	bool bIsFlip = false;
	FVector Direction = FVector::LEFT;
	bool bCanFly = false;

public:
		// 픽셀 충돌
		void SetOnGround(bool _Value)
		{
			bIsOnGround = _Value;
		}
		bool IsOnGround()
		{
			return bIsOnGround;
		}

		void SetWallHere(bool _Value)
		{
			bIsWallHere = _Value;
		}
		bool IsWallHere() const
		{
			return bIsWallHere;
		}

		void SetCeilHere(bool _Value)
		{
			bIsCeilHere = _Value;
		}
		bool GetCeilHere() const
		{
			return bIsCeilHere;
		}

		FVector GetGravityForce() const
		{
			return GravityForce;
		}
		void SetGravityForce(FVector _GravityForce)
		{
			GravityForce = _GravityForce;
		}

protected:
	// 픽셀 충돌
	bool bIsOnGround = false;
	bool bIsWallHere = false;
	bool bIsCeilHere = false;
	FVector GravityForce = FVector::ZERO;
	FVector GravityPointOffset = FVector::DOWN; // 렌더러와 이미지 위치가 차이가 날 경우
	FVector WallPointOffest = FVector::ZERO;
	FVector CeilPointOffset = FVector::ZERO;	

	void CheckCurRoom();
	void ActivePixelCollision();
	void ActiveGravity();

protected:
	USoundPlayer Sound;
	bool bIsSoundStop = false;
	bool bIsInitSound = false;

	float GetToPlayerDistance();

	std::string StaticSound = "BugWalk.wav";
	void PlayStaticSound(std::string_view _SoundFileName = "", float _MaxVolume = 0.5f, float _MaxDistance = 600.0f);

	std::string IdleSound = "";
	std::string AttackSound = "DashBugAttack.wav";
	std::string DeathSound = "enemy_death_sword.wav";
	void SoundPlay(std::string_view _SoundFileName, bool _IsLoop = false, bool _PrevSoundStop = true, float _Volume = 1.0f);
	
};

