#pragma once

struct FConst
{
	inline static const float Half = 0.5f;

	inline static const float MapZSort = -1000.0f;
	inline static const float MinoZSort = 1000.0f;

	inline static const FVector FirstPlayerPos = { -200, 0 };
	inline static const FPoint Point = { 10, 20 };
	
	
};

struct FGlobals
{
	// 테트로미노 생성자에서 값이 저장되므로 BeginPlay 이후에 값을 가져오는 것을 권장
	inline static FVector MinoSize = { 0, 0 }; 

};