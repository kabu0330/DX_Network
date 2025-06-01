#include "PreCompile.h"
#include "TetrisGameMode.h"

ATetrisGameMode::ATetrisGameMode()
{
}

ATetrisGameMode::~ATetrisGameMode()
{
}

void ATetrisGameMode::BeginPlay()
{
	AGameMode::BeginPlay();
}

void ATetrisGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);

}

void ATetrisGameMode::LevelChangeStart()
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

