#include "PreCompile.h"
#include "TetrisGameMode.h"
#include "TetrisPlayEditor.h"
#include "GameField.h"

ATetrisGameMode::ATetrisGameMode()
{
}

ATetrisGameMode::~ATetrisGameMode()
{
}

void ATetrisGameMode::BeginPlay()
{
	AGameMode::BeginPlay();

	AGameField* GameField = GetWorld()->SpawnActor<AGameField>().get();
}

void ATetrisGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);

}

void ATetrisGameMode::LevelChangeStart()
{
	UEngineGUI::AllWindowOff();
	{
		ServerEditor = UEngineGUI::FindGUIWindow<UTetrisPlayEditor>(UEngineString::AnsiToUTF8("네트워크_에디터"));
		if (nullptr == ServerEditor)
		{
			ServerEditor = UEngineGUI::CreateGUIWindow<UTetrisPlayEditor>(UEngineString::AnsiToUTF8("네트워크_에디터"));
		}

		ServerEditor->SetActive(true);
	}
}

