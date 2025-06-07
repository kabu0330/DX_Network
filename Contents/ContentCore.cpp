#include "PreCompile.h"
#include "ContentCore.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/HUD.h>

#include "DebugWindowGUI.h"
#include "ContentsEditorGUI.h"

#include "ContentResource.h"
#include "MapEditorGameMode.h"
#include "ThreadTestLevel.h"
#include "PostEffectGameMode.h"
#include "ServerGameMode.h"
#include "ServerPawn.h"

#include "TetrisGameMode.h"
#include "TetrisPlayEditor.h"
#include "Tetromino.h"
#include "TetrisPlayHUD.h"

#include "TestGameMode.h"

// 상위 엔진 레벨에 해당 클래스를 EngineCore에 간접적인 제어권한을 가지는 컨텐츠 코어로 설정한다.
CreateContentsCoreDefine(UContentCore);

void UContentCore::EngineStart(UEngineInitData& _Data)
{
	SetWindowSize(_Data);

	UContentResource::LoadResourceDirectory();
	UContentResource::LoadFolder();
	UContentResource::LoadSprite();
	UContentResource::LoadShaderResource();

	CreateLevel();
	OpenLevel();

	CreateEditior();

}

void UContentCore::SetWindowSize(UEngineInitData& _Data)
{
	_Data.WindowPos = { 50,0 };
	_Data.WindowSize = { 800, 600 };
}

void UContentCore::CreateLevel()
{
	//UEngineCore::CreateLevel<ATestGameMode, APawn, AHUD>("Title");
	//UEngineCore::CreateLevel<AThreadTestLevel, APawn, AHUD>("Test");
	//UEngineCore::CreateLevel<APostEffectGameMode, APawn, AHUD>("PostEffect");
	//UEngineCore::CreateLevel<AServerGameMode, AServerPawn, AHUD>("Server");
	UEngineCore::CreateLevel<ATetrisGameMode, ATetromino, ATetrisPlayHUD>("TetrisPlay");
	//UEngineCore::CreateLevel<AMapEditorGameMode, APawn, AHUD>("MapEditorMode");
}

void UContentCore::OpenLevel()
{
	UEngineCore::OpenLevel("TetrisPlay");
	//UEngineCore::OpenLevel("Server");
#ifdef _DEBUG

	//UEngineCore::OpenLevel("Server");
#else
	//UEngineCore::OpenLevel("Server");
#endif
}

void UContentCore::CreateEditior()
{
	UEngineGUI::AllWindowOff();

	{
		UEngineGUI::CreateGUIWindow<UContentsEditorGUI>(UEngineString::AnsiToUTF8("레벨"));
		std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>(UEngineString::AnsiToUTF8("레벨"));
		Window->SetActive(true);
		UEngineGUI::PushAlwaysOnGUI(Window); // 레벨이 넘어가도 해당 GUI는 다시 켜도록 예외처리
	}
	{
		UEngineGUI::CreateGUIWindow<UDebugWindowGUI>(UEngineString::AnsiToUTF8("디버그"));
		std::shared_ptr<UDebugWindowGUI> Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>(UEngineString::AnsiToUTF8("디버그"));
		Window->SetActive(true);
		UEngineGUI::PushAlwaysOnGUI(Window);
	}
}

void UContentCore::EngineTick(float _DeltaTime)
{
}

void UContentCore::EngineEnd()
{
}

UContentCore::UContentCore()
{
}

UContentCore::~UContentCore()
{
}