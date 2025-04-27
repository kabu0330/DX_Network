#pragma once
#include "Knight.h"
#include "Monster.h"
#include "StatusUnit.h"
#include <EnginePlatform/EngineSound.h>

// 설명 : 전투관련 상호작용 함수 집합
class UFightUnit
{
public:
	static void OnHit(AKnight* _Knight, int _Att);
	static void OnHit(AMonster* _Monster, int _Att);
	static void RecoverMp(int _Mp);


protected:

private:
	inline static USoundPlayer Sound = USoundPlayer();

	// constrcuter destructer
	UFightUnit();
	~UFightUnit();

	// delete Function
	UFightUnit(const UFightUnit& _Other) = delete;
	UFightUnit(UFightUnit&& _Other) noexcept = delete;
	UFightUnit& operator=(const UFightUnit& _Other) = delete;
	UFightUnit& operator=(UFightUnit&& _Other) noexcept = delete;
};

