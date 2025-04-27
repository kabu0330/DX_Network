#pragma once

// ���� : ������� ����
class RoomPos
{
public:
	// ���� �������� ������ �����η�
	inline static FVector FromDirtmouth = { 9300, -3408 }; // ������ǥ
	inline static FVector ToCrossroads1 = { 9350, -4000 }; 

	// ������ �����ο��� �ٽ� ������
	inline static FVector FromCrossroads1 = { 9384, -5595 };
	inline static FVector ToDirtmouth = { 9000, -3000 };

	// ������ 1 <-> 3 (����)
	inline static FVector Crossroads1Left = { 6210, -5740 };
	inline static FVector Crossroads3RightTop = { 5450, -5760 };

	// ������ 1 <-> 2 (����)
	inline static FVector Crossroads1Right = { 11940, -5445 };
	inline static FVector Crossroads2Left = { 12650, -5380 };

	// ������ 2 <-> 4 (����)
	inline static FVector Crossroads2Right = { 18240, -5400 };
	inline static FVector Crossroads4Left = { 18500, -5340 };

	// ������ 3 <-> 6 (�Ʒ��� ����)
	inline static FVector Crossroads3RightBot = { 5473, -8265 };
	inline static FVector Crossroads6Left = { 6074, -8265 };

	// ������6 <-> ������ (�Ʒ��� ����)
	inline static FVector Crossroads6Right = { 10726, -8190 };
	inline static FVector BossRoomLeft = { 11080, -8240 };

	// ������ <-> ������5 (����)
	inline static FVector BossRoomRight = { 14130, -8240 };
	inline static FVector Crossroads5LeftBot = { 14680, -8260 };

	// ������4 (���� ����) <-> ������5 (�Ʒ��� ����) 
	inline static FVector Crossroads4Right = { 24200, -5315 };
	inline static FVector Crossroads5LeftTop = { 18040, -7500 };

	// ����ó�� 1. ������5 ����
	inline static FVector Crossroads5Right = { 21790, -7650 };

	// ����ó�� 2. ������3 ���� 2��, �Ʒ� 1��
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
