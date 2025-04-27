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
	// 나이트
	_GameMode->GetWorld()->CreateCollisionProfile("Knight");
	_GameMode->GetWorld()->CreateCollisionProfile("KnightObject");

	// 몬스터
	_GameMode->GetWorld()->CreateCollisionProfile("Monster");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterObject");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterPoint");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterDetect");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterHead");

	// 기타 오브젝트
	_GameMode->GetWorld()->CreateCollisionProfile("Barrier");
	_GameMode->GetWorld()->CreateCollisionProfile("BossSpawnTrigger");

	// 워프
	_GameMode->GetWorld()->CreateCollisionProfile("Door");
}

void UCollisionManager::LinkCollision(AGameMode* _GameMode)
{
	// 충돌체크 해야한다.
	//_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Monster"); // 피격
	_GameMode->GetWorld()->LinkCollisionProfile("Door", "Knight"); // 워프
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Door"); // 워프 키입력

	// 발판
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Monster"); 
	_GameMode->GetWorld()->LinkCollisionProfile("BossSpawnTrigger", "Knight"); 

	// 나이트 공격
	_GameMode->GetWorld()->LinkCollisionProfile("KnightObject", "Monster"); 
	//_GameMode->GetWorld()->LinkCollisionProfile("KnightObject", "MonsterHead"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterHead", "KnightObject"); 

	// 몬스터 공격
	_GameMode->GetWorld()->LinkCollisionProfile("Monster", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterObject", "Knight");
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterDetect", "Knight");
}

