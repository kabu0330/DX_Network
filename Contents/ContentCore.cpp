#include "PreCompile.h"
#include "ContentCore.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/HUD.h>

#include "ContentResource.h"
#include "MapEditorGameMode.h"
#include "ContentsEditorGUI.h"
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

	UEngineGUI::AllWindowOff();

	UEngineGUI::CreateGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");
	std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");
	Window->SetActive(true);
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
#ifdef _DEBUG
	UEngineCore::OpenLevel("TetrisPlay");
#else
	UEngineCore::OpenLevel("Title");
#endif
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