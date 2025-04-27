#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/Pawn.h>
#include <EngineCore/TimeEventComponent.h>
#include "Global.h"
#include "StatusUnit.h"

// ���� : Player
class AKnight : public APawn
{
public:
	// constrcuter destructer
	AKnight();
	~AKnight();

	// delete Function
	AKnight(const AKnight& _Other) = delete;
	AKnight(AKnight&& _Other) noexcept = delete;
	AKnight& operator=(const AKnight& _Other) = delete;
	AKnight& operator=(AKnight&& _Other) noexcept = delete;

	void SetCameraPos();

	static AKnight* GetPawn()
	{
		return MainPawn;
	}

	USceneComponent* GetRootComponent() const
	{
		return RootComponent.get();
	}

	UContentsRenderer* GetRenderer() const
	{
		return BodyRenderer.get();
	}

	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

	bool IsLeft()
	{
		return bIsLeft;
	}
	bool IsKnockback()
	{
		return bIsKnockback;
	}

	void CheckEnterDoor(class UCollision* _This, class UCollision* _Target);
	bool GetEnter()
	{
		return bIsEnter;
	}

	UStatusUnit& GetStatRef()
	{
		return Stat;
	}
	UFSMStateManager& GetFSMRef()
	{
		return FSM;
	}

	void SetCanMove(bool _bCanMove)
	{
		bCanMove = _bCanMove;
	}


public:
	// �ȼ� �浹 �����
	void SwitchActiveGravity()
	{
		NoneGravity = !NoneGravity;
	}

	// �ȼ� �浹
	void SetOnGround(bool _Value)
	{
		bIsOnGround = _Value;
	}
	bool IsOnGround() const
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

	void SetBarrier(bool _bIsBarrier)
	{
		bIsBarrier = _bIsBarrier;
	}
	void GetBarrierPos(FVector _BarrierPos)
	{
		BarrierPos = _BarrierPos;
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
	UCollision* GetPixelCollision()
	{
		return PixelCollision.get();
	}

	void SetHardLand(bool _Value)
	{
		bIsHardLand = _Value;
	}

	float GetJumpForce()
	{
		return JumpForce;
	}
	
	bool IsInvincible()
	{
		return bIsInvincible;
	}
	void SetInvicible(bool _bIsInvincible)
	{
		bIsInvincible = _bIsInvincible;
	}

protected:
	bool bCanReset = false;
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TimeElapsed(float _DeltaTime); // ��Ÿ�� ����
	void CheckEnterButton();

	// �浹����
	void SetCollisionEvent();
	void Collide(class UCollision* _This, class UCollision* _Other);

	void DeathCheck();
	void ResetLevel();

	void UpdateFSM(float _DeltaTime);

private:
	bool bIsBarrier = false;
	FVector BarrierPos = FVector::ZERO;

	// Stat
	void SetStatus();
	UStatusUnit Stat = UStatusUnit();

	bool bIsEnter = false;

	// ���� �Ǵ� �ǰ� ���� ���� ��
	bool CanAction();
	bool CanMove();
	void RecoveryIdle();


	float HitStunDuration = 0.5f;
	float InvincibleDuration = 1.5f;
	float InvincibleElapsed = 0.0f;
	bool bIsInvincible = false;

	void ReverseForce(float _DeltaTime);
	void Knockback(float _DeltaTime);
	bool bIsOnGround = false; // �ȼ��浹�� true / false �˻�
	bool bIsKnockback = false;

	// Move
	void Move(float _DeltaTime);

	// Jump : ZŰ
	bool CanJump();
	void Jump(float _DeltaTime);
	bool bCanJump = true;
	FVector GravityForce = FVector::ZERO;
	float JumpForce = 0.0f;
	float InitJumpForce = 600.0f;

	// Slash : XŰ
	bool bIsShowEffect = false;
	float AttackCooldownElapsed = 0.0f;
	void CreateSlashEffect();
	void CreateUpSlashEffect();
	void CreateDownSlashEffect();

	// Dash : CŰ
	bool bIsDashing = false;
	bool bCanDash = true;
	bool bCanRotation = true; // ������ȯ ����, ��� �߿��� �Ұ�
	float DashCooldownElapsed = 0.0f;
	bool bIsDashEffect = false;
	void ChangeDash();
	void Dash();
	void CreateDashEffect();

	// Spell : AŰ
	bool bIsFireballEffect = false;
	void CastFireball();
	void CreateFireballEffect();
	bool bIsFocusEffect = false;
	void CastFocus();
	void CreateFocusEffect();
	bool bIsFocusEndEffect = false;
	void CreateFocusEndEffect();

	// �ǰ� : ����
	bool bIsStunEffect = false;
	float InitKnockbackDistance = 0.0f;
	void CreateStunEffect();

private:
	// �����
	void DebugInput(float _DeltaTime);
	void DebugCamera();
	void SwitchDebugMode()
	{
		bIsDebugMode = !bIsDebugMode;
	}
	bool bIsDebugMode = false;
	bool NoneGravity = false;
	static AKnight* MainPawn;

	UTimeEventComponent* TimeEventer = nullptr;

	// ī�޶�
	void SetCameraLerp();
	void CheckCameraPos();
	bool bIsCameraMove = false;
	FVector CameraPos = { 0.0f, 0.0f, 0.0f };
	FVector CameraCurPos = FVector::ZERO;
	FVector CameraTargetPos = FVector::ZERO;
	float CameraMoveTime = 0.0f;
	float ScreenRatioY = 0.0f;


	// Renderer
	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	void CreateRenderer();

	// Collision
	bool bIsWallHere = false;
	bool bIsCeilHere = false;
	std::shared_ptr<class UCollision> BodyCollision;
	std::shared_ptr<class UCollision> PixelCollision;
	void CreateCollision();
	void CreatePixelCollision();

	// Animation
	void InitAnimation();
	void ChangeNextState(EKnightState _NextState);
	void ChangePrevState();

	void ChangeAttackAnimation(EKnightState _PrevState);
	void ChangeJumpAnimation();
	void ChangeNonCombatAnimation();
	void ChangeLookAnimation();

	// FSM
	using StateCallback = void(AKnight::*)(float);
	void CreateState(EKnightState _State, StateCallback _Callback, std::string_view _AnimationName);

	void SetFSM();
	UFSMStateManager FSM;
	EKnightState NextState = EKnightState::IDLE;

	void SetIdle(float _DeltaTime);
	void SetRun(float _DeltaTime);
	void SetIdleToRun(float _DeltaTime);
	void SetRunToIdle(float _DeltaTime);
	void SetDash(float _DeltaTime);
	void SetJump(float _DeltaTime);
	void SetAirborne(float _DeltaTime);
	void SetLand(float _DeltaTime);
	void SetHardLand(float _DeltaTime);

	void SetLookDown(float _DeltaTime);
	void SetLookDownLoop(float _DeltaTime);
	void SetLookUp(float _DeltaTime);
	void SetLookUpLoop(float _DeltaTime);

	void SetSlash(float _DeltaTime);
	void SetUpSlash(float _DeltaTime);
	void SetDownSlash(float _DeltaTime);

	void SetFocus(float _DeltaTime);
	void SetFocusGet(float _DeltaTime);
	void SetFocusEnd(float _DeltaTime);
	void SetFireballAntic(float _DeltaTime);
	void SetFireballCast(float _DeltaTime);

	void SetStun(float _DeltaTime);

	void SetDeath(float _DeltaTime);
	void SetDeathDamage(float _DeltaTime);
	void SetDeathHead(float _DeltaTime);

	void CheckDirection();
	bool bIsLeft = true;
	bool bIsHardLand = false;
	float MinAirborneTrigger = 100.0f;
	bool bIsLand = false;
	bool bWasHardLanding = false;
	bool bCanMove = true; // ���� ����, ���� �̵� ���ܿ�


	// �ȼ��浹
	void ActiveGravity();
	void ActivePixelCollsion();

	USoundPlayer Sound;
	bool bIsSound = false;
	bool bIsDeathSound = false;
};

