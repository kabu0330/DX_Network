#include "PreCompile.h"
#include "CollisionManager.h"

UCollisionManager::UCollisionManager()
{
}

UCollisionManager::~UCollisionManager()
{
}

void UCollisionManager::CreateCollisionProfile(AGameMode* _GameMode)
{
	// ����Ʈ
	_GameMode->GetWorld()->CreateCollisionProfile("Knight");
	_GameMode->GetWorld()->CreateCollisionProfile("KnightObject");

	// ����
	_GameMode->GetWorld()->CreateCollisionProfile("Monster");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterObject");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterPoint");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterDetect");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterHead");

	// ��Ÿ ������Ʈ
	_GameMode->GetWorld()->CreateCollisionProfile("Barrier");
	_GameMode->GetWorld()->CreateCollisionProfile("BossSpawnTrigger");

	// ����
	_GameMode->GetWorld()->CreateCollisionProfile("Door");
}

void UCollisionManager::LinkCollision(AGameMode* _GameMode)
{
	// �浹üũ �ؾ��Ѵ�.
	//_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Monster"); // �ǰ�
	_GameMode->GetWorld()->LinkCollisionProfile("Door", "Knight"); // ����
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Door"); // ���� Ű�Է�

	// ����
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Monster"); 
	_GameMode->GetWorld()->LinkCollisionProfile("BossSpawnTrigger", "Knight"); 

	// ����Ʈ ����
	_GameMode->GetWorld()->LinkCollisionProfile("KnightObject", "Monster"); 
	//_GameMode->GetWorld()->LinkCollisionProfile("KnightObject", "MonsterHead"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterHead", "KnightObject"); 

	// ���� ����
	_GameMode->GetWorld()->LinkCollisionProfile("Monster", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterObject", "Knight");
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterDetect", "Knight");
}

