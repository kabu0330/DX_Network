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

void UDebugWindowGUI::OnGUI(float _DelataTime)
{
	if (true == ImGui::Button("FreeCameraOn"))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();

	}

	GetFrame();
	GetMousePos();
}

void UDebugWindowGUI::GetFrame()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	int Result = static_cast<int>(1.0f / DeltaTime);
	ImGui::Text("Frame : %d", Result);
}

void UDebugWindowGUI::GetMousePos()
{
	ACameraActor* Camera = GetWorld()->GetMainCamera().get();
	if (nullptr == Camera)
	{
		return;
	}

	FVector MousePos = Camera->ScreenMousePosToWorldPos();
	ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);

	//FVector MapPos = ARoom::GetCurRoom()->GetLeftTopPos();
	//ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X - MapPos.X, MousePos.Y - MapPos.Y);
}


