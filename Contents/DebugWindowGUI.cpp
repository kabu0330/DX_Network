#include "PreCompile.h"
#include "DebugWindowGUI.h"
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include "PlayGameMode.h"
#include "Room.h"

UDebugWindowGUI::UDebugWindowGUI()
{
}

UDebugWindowGUI::~UDebugWindowGUI()
{
}

void UDebugWindowGUI::OnGUI()
{
	if (true == ImGui::Button("FreeCameraOn"))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();

	}

	GetFrame();
	GetCurRoom();
	GetMousePos();
	GetKnightInfo();
	GetGravityForce();
}

void UDebugWindowGUI::GetMousePos()
{
	FVector MousePos = APlayGameMode::MousePos;
	ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);

	FVector MapPos = ARoom::GetCurRoom()->GetLeftTopPos();
	ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X - MapPos.X, MousePos.Y - MapPos.Y);
}

void UDebugWindowGUI::GetCurRoom()
{
	if (nullptr == ARoom::GetCurRoom())
	{
		return;
	}
	std::string CurRoomName = ARoom::GetCurRoom()->GetName();
	ImGui::Text("CurRoom Name : %s", CurRoomName.c_str());
}

void UDebugWindowGUI::GetKnightInfo()
{
	AKnight* Knight = AKnight::GetPawn();
	FVector KnightPos = APlayGameMode::KnightPos;
	ImGui::Text("Knight Pos X : %.0f, Y : %.0f", KnightPos.X, KnightPos.Y);

	float ZValue = AKnight::GetPawn()->GetRenderer()->GetTransformRef().RelativeLocation.Z;
	ImGui::Text("Knight Z Vaule : %.6f", ZValue);

	ImGui::Text("Knight JumpPower : %.2f", Knight->GetJumpForce());
}

void UDebugWindowGUI::GetGravityForce()
{
	//ImGui::Text("Knight GravityForce : %.2f", AKnight::GetPawn()->GravityForce);
	int Result = static_cast<int>(AKnight::GetPawn()->IsOnGround());
	std::string ResultString = "";
	if (0 == Result)
	{
		ResultString = "false";
	}
	else
	{
		ResultString = "true";
	}
	ImGui::Text("Knight IsOnGround : %s", ResultString.data());
}

void UDebugWindowGUI::GetFrame()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	int Result = static_cast<int>(1.0f / DeltaTime);
	ImGui::Text("Frame : %d", Result);
}
