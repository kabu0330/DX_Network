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
	// ��Ʈ�ι̳� �����ڿ��� ���� ����ǹǷ� BeginPlay ���Ŀ� ���� �������� ���� ����
	inline static FVector MinoSize = { 0, 0 }; 

};