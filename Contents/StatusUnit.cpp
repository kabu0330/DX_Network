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
	// �̵��ӵ� ����
	Velocity = _Data->Velocity;
	InitVelocity = _Data->InitVelocity;
	DashSpeed = _Data->DashSpeed;
	RunSpeed = _Data->RunSpeed;

	// ü�°���
	MaxHp = _Data->MaxHp;
	Hp = _Data->Hp;

	// ���� ����
	MaxMp = _Data->MaxMp;
	Mp = _Data->Mp;

	// ���ݹ��� ����
	Att = _Data->Att;
	SpellAtt = _Data->SpellAtt;

	// �ǰ� ����
	bIsKnockbackable = _Data->bIsKnockbackable;
	KnockbackDistance = _Data->KnockbackDistance;

	// ��ȭ
	Geo = _Data->Geo;


	if (0 > Velocity)
	{
		MSGASSERT("�̵��ӵ��� ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}
	if (1 > Hp)
	{
		MSGASSERT("ü���� �ּ� 1 �̻����� �������ּ���.");
		return false;
	}
	if (Hp > MaxHp)
	{
		MSGASSERT("�ִ� ü�º��� HP�� �� ���� ������ �� �����ϴ�.");
		return false;
	}
	if (0 > Mp)
	{
		MSGASSERT("������ ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}
	if (Mp > MaxMp)
	{
		MSGASSERT("�ִ� �������� MP�� �� ���� ������ �� �����ϴ�.");
		return false;
	}
	if (1 > Att)
	{
		MSGASSERT("���ݷ��� �ּ� 1 �̻����� �������ּ���.");
		return false;
	}
	if (0 > SpellAtt)
	{
		MSGASSERT("���� ���ݷ��� ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}

	return true;
}


