#include "PreCompile.h"
#include "StatusUnit.h"

UStatusUnit::UStatusUnit()
{
}

UStatusUnit::~UStatusUnit()
{
}

bool UStatusUnit::CreateStatus(FStatusData* _Data)
{
	// 이동속도 관련
	Velocity = _Data->Velocity;
	InitVelocity = _Data->InitVelocity;
	DashSpeed = _Data->DashSpeed;
	RunSpeed = _Data->RunSpeed;

	// 체력관련
	MaxHp = _Data->MaxHp;
	Hp = _Data->Hp;

	// 마나 관련
	MaxMp = _Data->MaxMp;
	Mp = _Data->Mp;

	// 공격범위 관련
	Att = _Data->Att;
	SpellAtt = _Data->SpellAtt;

	// 피격 관련
	bIsKnockbackable = _Data->bIsKnockbackable;
	KnockbackDistance = _Data->KnockbackDistance;

	// 재화
	Geo = _Data->Geo;


	if (0 > Velocity)
	{
		MSGASSERT("이동속도를 음수로 설정할 수 없습니다. 최소 0 이상으로 설정해주세요.");
		return false;
	}
	if (1 > Hp)
	{
		MSGASSERT("체력은 최소 1 이상으로 설정해주세요.");
		return false;
	}
	if (Hp > MaxHp)
	{
		MSGASSERT("최대 체력보다 HP가 더 높게 설정할 수 없습니다.");
		return false;
	}
	if (0 > Mp)
	{
		MSGASSERT("마나를 음수로 설정할 수 없습니다. 최소 0 이상으로 설정해주세요.");
		return false;
	}
	if (Mp > MaxMp)
	{
		MSGASSERT("최대 마나보다 MP가 더 높게 설정할 수 없습니다.");
		return false;
	}
	if (1 > Att)
	{
		MSGASSERT("공격력은 최소 1 이상으로 설정해주세요.");
		return false;
	}
	if (0 > SpellAtt)
	{
		MSGASSERT("마법 공격력을 음수로 설정할 수 없습니다. 최소 0 이상으로 설정해주세요.");
		return false;
	}

	return true;
}


