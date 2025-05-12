#include "PreCompile.h"
#include "DebugWindowGUI.h"
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>

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
	//FVector MousePos = APlayGameMode::MousePos;
	//ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);

	//FVector MapPos = ARoom::GetCurRoom()->GetLeftTopPos();
	//ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X - MapPos.X, MousePos.Y - MapPos.Y);
}

void UDebugWindowGUI::GetCurRoom()
{

}

void UDebugWindowGUI::GetKnightInfo()
{

}

void UDebugWindowGUI::GetGravityForce()
{

}

void UDebugWindowGUI::GetFrame()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	int Result = static_cast<int>(1.0f / DeltaTime);
	ImGui::Text("Frame : %d", Result);
}
