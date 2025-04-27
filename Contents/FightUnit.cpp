#include "PreCompile.h"
#include "FightUnit.h"

UFightUnit::UFightUnit()
{
}

UFightUnit::~UFightUnit()
{
}

void UFightUnit::OnHit(AKnight* _Knight, int _Att)
{
	if (true == _Knight->IsInvincible())
	{
		return;
	}
	UStatusUnit& Stat = _Knight->GetStatRef();
	Stat.AddHp(-_Att); // 체력 감소

	// 스턴 상태
	Stat.SetStun(true);
	Stat.SetBeingHit(true);
	_Knight->GetFSMRef().ChangeState(EKnightState::STUN);
}

void UFightUnit::OnHit(AMonster* _Monster, int _Att)
{
	UStatusUnit& Stat = _Monster->GetStatRef();
	Stat.AddHp(-_Att);

	Stat.SetStun(true);
	Stat.SetBeingHit(true);
	Sound = UEngineSound::Play("enemy_damage.wav");
	//_Monster->GetFSMRef().ChangeState(EMonsterState::HIT);
}

void UFightUnit::RecoverMp(int _Mp)
{
	AKnight* Knight = AKnight::GetPawn();
	Knight->GetStatRef().AddMp(_Mp); // 예외처리는 AddMp에서 한다.

}

