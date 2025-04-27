#pragma once

struct FStatusData
{
	// 이동속도 관련
	float Velocity = 100.0f;
	float RunSpeed = 0.0f; // 몬스터만
	float InitVelocity = 0.0f;
	float DashSpeed = 0.0f;

	// 체력관련
	int MaxHp = 5;
	int Hp = 5;

	// 마나 관련
	int MaxMp = 99;
	int Mp = 0;

	// 공격범위 관련
	int Att = 1;
	int SpellAtt = 1;

	// 피격 관련
	bool bIsKnockbackable = true;
	float KnockbackDistance = 50.0f;

	// 재화
	int Geo = 0;
};

// 설명 : 플레이어와 몬스터의 스텟을 설정하고 관리
class UStatusUnit
{
public:
	// constrcuter destructer
	UStatusUnit();
	~UStatusUnit();

	// delete Function
	UStatusUnit(const UStatusUnit& _Other) = delete;
	UStatusUnit(UStatusUnit&& _Other) noexcept = delete;
	UStatusUnit& operator=(const UStatusUnit& _Other) = delete;
	UStatusUnit& operator=(UStatusUnit&& _Other) noexcept = delete;

	bool CreateStatus(FStatusData* _Data);

	float GetVelocity()
	{
		return Velocity;
	}
	void SetVelocity(float _Velocity)
	{
		Velocity = _Velocity;
	}

	float GetInitVelocity()
	{
		return InitVelocity;
	}
	void SetInitVelocity(float _InitVelocity)
	{
		InitVelocity = _InitVelocity;
	}

	float GetRunSpeed()
	{
		return RunSpeed;
	}
	void SetRunSpeed(float _RunSpeed)
	{
		RunSpeed = _RunSpeed;
	}

	float GetDashSpeed()
	{
		return DashSpeed;
	}
	void SetDashSpeed(float _DashSpeed)
	{
		DashSpeed = _DashSpeed;
	}

	int GetMaxHp()
	{
		return MaxHp;
	}
	void SetMaxHp(int _MaxHP)
	{
		MaxHp = _MaxHP;
	}

	int GetHp()
	{
		return Hp;
	}
	int& GetHpRef()
	{
		return Hp;
	}
	void AddHp(int _HP)
	{
		Hp += _HP;
		if (Hp > MaxHp)
		{
			Hp = MaxHp;
			return;
		}
		else if (Hp < 0)
		{
			Hp = 0;
			return;
		}
	}

	int GetMaxMp()
	{
		return MaxMp;
	}
	void SetMaxMp(int _MaxMP)
	{
		MaxMp = _MaxMP;
	}

	int GetMp()
	{
		return Mp;
	}
	void AddMp(int _MP)
	{
		Mp += _MP;
		if (Mp > MaxMp)
		{
			Mp = MaxMp;
			return;
		}
		else if (Mp < 0)
		{
			Mp = 0;
			return;
		}
	}

	int GetAtt()
	{
		return Att;
	}
	void SetAtt(int _Att)
	{
		Att = _Att;
	}

	int GetSpellAtt()
	{
		return SpellAtt;
	}
	void SetSpellAtt(int _SpellAtt)
	{
		SpellAtt = _SpellAtt;
	}

	bool IsKnockbackable()
	{
		return bIsKnockbackable;
	}
	void SetKnockbackable(bool _bIsKnockbackable)
	{
		bIsKnockbackable = _bIsKnockbackable;
	}

	bool IsAttacking()
	{
		return bIsAttacking;
	}
	void SetAttacking(bool _bIsAttacking)
	{
		bIsAttacking = _bIsAttacking;
	}

	bool IsBeingHit()
	{
		return bIsBeingHit;
	}
	void SetBeingHit(bool _bIsBeingHit)
	{
		bIsBeingHit = _bIsBeingHit;
	}

	bool IsCastingSpell()
	{
		return bIsCastingSpell;
	}
	void SetIsCastingSpell(bool _bIsCastingSpell)
	{
		bIsCastingSpell = _bIsCastingSpell;
	}

	bool IsStun()
	{
		return bIsStun;
	}
	void SetStun(bool _bIsStun)
	{
		bIsStun = _bIsStun;
	}

	bool IsDeath()
	{
		return bIsDeath;
	}
	void SetDeath(bool _bIsDeath)
	{
		bIsDeath = _bIsDeath;
	}

	FVector GetKnockbackForce()
	{
		return KnockbackForce;
	}
	void SetKnockbackDir(FVector _KnockbackDir)
	{
		KnockbackForce = _KnockbackDir * KnockbackDistance;
	}
	void AddKnockbackForce(FVector _KnockbackForce)
	{
		KnockbackForce += _KnockbackForce;
	}
	void SetKnockbackForce(FVector _KnockbackForce)
	{
		KnockbackForce = _KnockbackForce;
	}

	float GetKnockbackDistance()
	{
		return KnockbackDistance;
	}
	void SetKnockbackDistance(float _KnockbackDistance)
	{
		KnockbackDistance = _KnockbackDistance;
	}
	

protected:

private:
	// 이동속도 관련
	float Velocity = 100.0f;
	float InitVelocity = 0.0f;
	float RunSpeed = 0.0f;
	float DashSpeed = 0.0f;

	// 체력관련
	int MaxHp = 5;
	int Hp = 5;

	// 마나 관련
	int MaxMp = 99;
	int Mp = 0;

	// 공격력 
	int Att = 1;
	int SpellAtt = 1;

	// 피격 
	bool bIsAttacking = false;
	bool bIsBeingHit = false;
	bool bIsCastingSpell = false;
	bool bIsStun = false;
	bool bIsDeath = false;
	bool bIsKnockbackable = true;
	float KnockbackDistance = 100.0f;
	FVector KnockbackForce = FVector::ZERO;
	
	// 재화
	int Geo = 0;
};

