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

// ���� ���� ������ �ش� Ŭ������ EngineCore�� �������� ��������� ������ ������ �ھ�� �����Ѵ�.
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
		UEngineGUI::CreateGUIWindow<UContentsEditorGUI>(UEngineString::AnsiToUTF8("����"));
		std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>(UEngineString::AnsiToUTF8("����"));
		Window->SetActive(true);
		UEngineGUI::PushAlwaysOnGUI(Window); // ������ �Ѿ�� �ش� GUI�� �ٽ� �ѵ��� ����ó��
	}
	{
		UEngineGUI::CreateGUIWindow<UDebugWindowGUI>(UEngineString::AnsiToUTF8("�����"));
		std::shared_ptr<UDebugWindowGUI> Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>(UEngineString::AnsiToUTF8("�����"));
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