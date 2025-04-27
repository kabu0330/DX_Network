#pragma once

// 설명 : 상수값을 정의
class RoomPos
{
public:
	// 흙의 마을에서 잊혀진 교차로로
	inline static FVector FromDirtmouth = { 9300, -3408 }; // 월드좌표
	inline static FVector ToCrossroads1 = { 9350, -4000 }; 

	// 잊혀진 교차로에서 다시 마을로
	inline static FVector FromCrossroads1 = { 9384, -5595 };
	inline static FVector ToDirtmouth = { 9000, -3000 };

	// 교차로 1 <-> 3 (좌측)
	inline static FVector Crossroads1Left = { 6210, -5740 };
	inline static FVector Crossroads3RightTop = { 5450, -5760 };

	// 교차로 1 <-> 2 (우측)
	inline static FVector Crossroads1Right = { 11940, -5445 };
	inline static FVector Crossroads2Left = { 12650, -5380 };

	// 교차로 2 <-> 4 (우측)
	inline static FVector Crossroads2Right = { 18240, -5400 };
	inline static FVector Crossroads4Left = { 18500, -5340 };

	// 교차로 3 <-> 6 (아랫층 우측)
	inline static FVector Crossroads3RightBot = { 5473, -8265 };
	inline static FVector Crossroads6Left = { 6074, -8265 };

	// 교차로6 <-> 보스방 (아랫층 우측)
	inline static FVector Crossroads6Right = { 10726, -8190 };
	inline static FVector BossRoomLeft = { 11080, -8240 };

	// 보스방 <-> 교차로5 (우측)
	inline static FVector BossRoomRight = { 14130, -8240 };
	inline static FVector Crossroads5LeftBot = { 14680, -8260 };

	// 교차로4 (위층 우측) <-> 교차로5 (아랫층 좌측) 
	inline static FVector Crossroads4Right = { 24200, -5315 };
	inline static FVector Crossroads5LeftTop = { 18040, -7500 };

	// 예외처리 1. 교차로5 우측
	inline static FVector Crossroads5Right = { 21790, -7650 };

	// 예외처리 2. 교차로3 좌측 2개, 아래 1개
	inline static FVector Crossroads3LeftTop = { 2766, -5750 };
	inline static FVector Crossroads3LeftBot = { 2533, -8530 };
	inline static FVector Crossroads3Bot = { 3780, -11080 };
	inline static FVector Crossroads3BotWarpPoint = { 4760, -5860 };


	inline static FVector BossRoomBarrierLeft = { 11150, -8200 };
	inline static FVector BossRoomBarrierRight = { 13130, -8200 };
};

class InitPos
{
public:
	inline static FVector KnightInitPos = { 600, -1300 };
	inline static FVector Dirtmouth_well = { 9000, -3000 };
	inline static FVector CrossroadsEntrance = { 9390, -5000 };
	inline static FVector BossRoom = { 11800, -8000 };
};
