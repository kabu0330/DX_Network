#include "PreCompile.h"
#include "TetrisGameMode.h"

TetrisGameMode::TetrisGameMode()
{
}

TetrisGameMode::~TetrisGameMode()
{
}

void TetrisGameMode::BeginPlay()
{
	AGameMode::BeginPlay();
}

void TetrisGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);

}

void TetrisGameMode::LevelChangeStart()
{
	UEngineGUI::AllWindowOff();
	{
		ServerEditor = UEngineGUI::FindGUIWindow<UServerEditor>(UEngineString::AnsiToUTF8("네트워크_에디터"));
		if (nullptr == ServerEditor)
		{
			ServerEditor = UEngineGUI::CreateGUIWindow<UServerEditor>(UEngineString::AnsiToUTF8("네트워크_에디터"));
		}

		ServerEditor->SetActive(true);
	}
}

