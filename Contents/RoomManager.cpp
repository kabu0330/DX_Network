#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"
#include "Door.h"
#include "WanderingHusk.h"
#include "LeapingHusk.h"
#include "HuskHornhead.h"
#include "Crawlid.h"
#include "Vengefly.h"
#include "Gruzzer.h"
#include "AspidMother.h"
#include "AspidHatchling.h"
#include "AspidHunter.h"
#include "FalseKnight.h"
#include "Barrier.h"

URoomManager::URoomManager()
	: Rooms()
{
}

URoomManager::~URoomManager()
{
}

void URoomManager::CreateRoom(AGameMode* _GameMode)
{
	GameMode = _GameMode;
	std::string PNG = "_back.png";
	std::string BMP = "_pixel.bmp";
	std::string DirtmouthStr = "Dirtmouth";
	std::string CrossroadsStr1 = "ForgottenCrossroads1";
	std::string CrossroadsStr2 = "ForgottenCrossroads2";
	std::string CrossroadsStr3 = "ForgottenCrossroads3";
	std::string CrossroadsStr4 = "ForgottenCrossroads4";
	std::string CrossroadsStr5 = "ForgottenCrossroads5";
	std::string CrossroadsStr6 = "ForgottenCrossroads6";
	std::string FalseKnightRoom = "FalseKnightRoom"; // ������

	// �� ����
	// 1. ����
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	std::shared_ptr<ARoom> Crossroads1 = CreateRoom(CrossroadsStr1, CrossroadsStr1 + PNG, CrossroadsStr1 + BMP, { 5878, 2826 });
	std::shared_ptr<ARoom> Crossroads2 = CreateRoom(CrossroadsStr2, CrossroadsStr2 + PNG, CrossroadsStr2 + BMP, { 5752, 2470 });
	std::shared_ptr<ARoom> Crossroads3 = CreateRoom(CrossroadsStr3, CrossroadsStr3 + PNG, CrossroadsStr3 + BMP, { 3150, 6746 });
	std::shared_ptr<ARoom> Crossroads4 = CreateRoom(CrossroadsStr4, CrossroadsStr4 + PNG, CrossroadsStr4 + BMP, { 5904, 1574 });
	std::shared_ptr<ARoom> Crossroads5 = CreateRoom(CrossroadsStr5, CrossroadsStr5 + PNG, CrossroadsStr5 + BMP, { 7292, 1946 });
	std::shared_ptr<ARoom> Crossroads6 = CreateRoom(CrossroadsStr6, CrossroadsStr6 + PNG, CrossroadsStr6 + BMP, { 4794, 1578 });
	std::shared_ptr<ARoom> BossRoom = CreateRoom(FalseKnightRoom, FalseKnightRoom + PNG, FalseKnightRoom + BMP, { 3188, 1348 });

	Crossroads1->SetSound("Crossroads.mp3");

	// 2. push_back
	Rooms.reserve(8);
	Rooms.push_back(Dirtmouth);
	Rooms.push_back(Crossroads1);
	Rooms.push_back(Crossroads2);
	Rooms.push_back(Crossroads3);
	Rooms.push_back(Crossroads4);
	Rooms.push_back(Crossroads5);
	Rooms.push_back(Crossroads6);
	Rooms.push_back(BossRoom);

	// 3. �ȼ��浹 ��ġ����
	Dirtmouth->SetRoomLocation({ 0, 0 });
	// ����
	Crossroads1->SetRoomLocation({ 6150, -3550 });
	Crossroads2->SetRoomLocation({ 12572, -3580 });
	Crossroads4->SetRoomLocation({ 18400, -4228 });

	// ����
	Crossroads3->SetRoomLocation({ 2400, -4450 });
	
	// �Ʒ��� ���� -> ������
	Crossroads6->SetRoomLocation({ 6000, -7100 });
	BossRoom->SetRoomLocation({ 11000, -7200 });
	Crossroads5->SetRoomLocation({ 14600, -6800 });

	// 4. ��Ż or �� ����
	FVector DoorSize = { 50, 400 };
	//                      �� ũ��               �� ��ġ                    ���� ��             ���� ��ġ              �Է� Ű ��������?
	// ���Ǹ��� <-> ������1
	  Dirtmouth->CreateDoor({ 300, 100 }, RoomPos::FromDirtmouth,   Crossroads1.get(), RoomPos::ToCrossroads1      );
	Crossroads1->CreateDoor({ 100, 150 }, RoomPos::FromCrossroads1 , Dirtmouth.get(),   RoomPos::ToDirtmouth    , true); // ����Ű �Է����� ��Ż

	FVector Offset = { 100.0f, 0.0f };
	// ����
	// ������1 <-> ������2 (����)
	Crossroads1->CreateDoor(DoorSize, RoomPos::Crossroads1Right, Crossroads2.get(), RoomPos::Crossroads2Left + Offset);
	Crossroads2->CreateDoor(DoorSize, RoomPos::Crossroads2Left, Crossroads1.get(), RoomPos::Crossroads1Right - Offset);
	
	// ������2 <-> ������4
	Crossroads2->CreateDoor(DoorSize, RoomPos::Crossroads2Right, Crossroads4.get(), RoomPos::Crossroads4Left + Offset);
	Crossroads4->CreateDoor(DoorSize, RoomPos::Crossroads4Left, Crossroads2.get(), RoomPos::Crossroads2Right - Offset);

	// ������1 <-> ������3 (����)
	Crossroads1->CreateDoor(DoorSize,     RoomPos::Crossroads1Left , Crossroads3.get(), RoomPos::Crossroads3RightTop - Offset);
	Crossroads3->CreateDoor(DoorSize, RoomPos::Crossroads3RightTop, Crossroads1.get(), RoomPos::Crossroads1Left + Offset);


	// �Ʒ���
	// ������3 <-> ������6 (����)
	Crossroads3->CreateDoor(DoorSize, RoomPos::Crossroads3RightBot, Crossroads6.get(), RoomPos::Crossroads6Left + Offset);
	Crossroads6->CreateDoor(DoorSize, RoomPos::Crossroads6Left, Crossroads3.get(), RoomPos::Crossroads3RightBot - Offset);

	// ������6 <-> ������ (����)
	Crossroads6->CreateDoor(DoorSize, RoomPos::Crossroads6Right, BossRoom.get(), RoomPos::BossRoomLeft + Offset);
	BossRoom->CreateDoor(DoorSize, RoomPos::BossRoomLeft, Crossroads6.get(), RoomPos::Crossroads6Right - Offset);

	// ������ <-> ������5 (����)
	BossRoom->CreateDoor(DoorSize, RoomPos::BossRoomRight, Crossroads5.get(), RoomPos::Crossroads5LeftBot + Offset);
	Crossroads5->CreateDoor(DoorSize, RoomPos::Crossroads5LeftBot, BossRoom.get(), RoomPos::BossRoomRight - Offset);

	// ������ 4 <-> ������5 (��/�Ʒ� ��)
	Crossroads4->CreateDoor(DoorSize, RoomPos::Crossroads4Right, Crossroads5.get(), RoomPos::Crossroads5LeftTop + Offset);
	Crossroads5->CreateDoor(DoorSize, RoomPos::Crossroads5LeftTop, Crossroads4.get(), RoomPos::Crossroads4Right - Offset);

	// �̱��� �� ���� ó��
	Crossroads3->CreateDoor(DoorSize, RoomPos::Crossroads3LeftBot, Crossroads3.get(), RoomPos::Crossroads3LeftBot + Offset);
	Crossroads3->CreateDoor(DoorSize, RoomPos::Crossroads3LeftTop, Crossroads3.get(), RoomPos::Crossroads3LeftTop + Offset);
	Crossroads3->CreateDoor({300, 200}, RoomPos::Crossroads3Bot, Crossroads3.get(), RoomPos::Crossroads3BotWarpPoint);

	Crossroads5->CreateDoor(DoorSize, RoomPos::Crossroads5Right, Crossroads5.get(), RoomPos::Crossroads5Right - Offset);


	// ������ �踮��

	BossRoom->CreateBarrier(RoomPos::BossRoomBarrierLeft);
	BossRoom->CreateBarrier(RoomPos::BossRoomBarrierRight);
	
	// ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});

	// 5. ���� ��ġ �߷� ���� : ���� �浹�ؾ߸� �ȼ��浹 ���� �����Ǳ� ������ ���⼭ ������ ���� �ʿ��� ��� ���

	ARoom::GetCurRoom();

#ifdef _DEBUG
	//SetInitCurRoom(Crossroads1.get());
	//SetInitCurRoom(Crossroads1.get());
	SetInitCurRoom(BossRoom.get());
#else
	SetInitCurRoom(Dirtmouth.get());
#endif // _DEBUG




	// 6. ���� ����
	Crossroads3->CreateMonster<AGruzzer>({ 4400, -5970 });
	Crossroads3->CreateMonster<AGruzzer>({ 2940, -7700 });
	Crossroads3->CreateMonster<AGruzzer>({ 3660, -8060 });
	Crossroads3->CreateMonster<AGruzzer>({ 3730, -9870 });
	Crossroads3->CreateMonster<AGruzzer>({ 4620, -6700 });

	//Crossroads1->CreateMonster<AWanderingHusk>({ 11270, -5800 });
	Crossroads1->CreateMonster<AWanderingHusk>({ 9000, -6000 });
	Crossroads1->CreateMonster<ACrawlid>({ 7000, -5930 });

	Crossroads2->CreateMonster<AAspidHunter>({ 14300, -5200 });
	Crossroads2->CreateMonster<AAspidHunter>({ 16700, -5200 });

	Crossroads4->CreateMonster<AWanderingHusk>({ 19550, -5350 });
	Crossroads4->CreateMonster<AWanderingHusk>({ 23000, -5350 });
	Crossroads4->CreateMonster<AHuskHornhead>({ 22000, -5350 });

	Crossroads6->CreateMonster<AVengefly>({ 9070, -8100 });
	//Crossroads6->CreateMonster<AHuskHornhead>({ 8100, -8290 });
	Crossroads6->CreateMonster<ACrawlid>({ 8720, -8350 });
	Crossroads6->CreateMonster<ACrawlid>({ 7200, -8400 });

	BossRoom->CreateMonster<AFalseKnight>({ 12200, -7300 });

	//Crossroads5->CreateMonster<AAspidMother>({ 19700, -8260 });
	Crossroads5->CreateMonster<AAspidMother>({ 20300, -8260 });
	//Crossroads5->CreateMonster<ALeapingHusk>({ 16730, -8300 });
	//Crossroads5->CreateMonster<AHuskHornhead>({ 17000, -8300 });
	Crossroads5->CreateMonster<ACrawlid>({ 17000, -8300 });



}

std::shared_ptr<ARoom> URoomManager::CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio/* = 1.0f*/)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	float ZOrder = static_cast<float>(EZOrder::BACKGROUND);
	NewRoom->SetActorLocation({ _Size.X / 2.0f , -_Size.Y / 2.0f, ZOrder });

	NewRoom->CreateTexture(_BackgroundName, _ScaleRatio);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _PixelCollisionName, _Size, _ScaleRatio);
	return NewRoom;
}

void URoomManager::SetInitCurRoom(ARoom* _InitRoom)
{
	ARoom::SetCurRoom(_InitRoom);
}

void URoomManager::LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size, float _ScaleRatio)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append("MapData");
	UEngineFile ImageFiles = Dir.GetFile(_FileName);

	_BmpTexture->Load(nullptr, ImageFiles.GetPathToString()); // �ȼ� �浹 ���� ����

}

