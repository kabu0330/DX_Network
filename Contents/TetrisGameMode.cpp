#include "PreCompile.h"
#include "TetrisGameMode.h"
#include "TetrisPlayEditor.h"
#include "GameField.h"

ATetrisGameMode::ATetrisGameMode()
{
	GetWorld()->CreateCollisionProfile("Debug");
}

ATetrisGameMode::~ATetrisGameMode()
{
}

void ATetrisGameMode::BeginPlay()
{
	AGameMode::BeginPlay();

	GameField = GetWorld()->SpawnActor<AGameField>().get();
}

void ATetrisGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);

}

void ATetrisGameMode::LevelChangeStart()
{
	//UEngineGUI::AllWindowOff();
	{
		ServerEditor = UEngineGUI::FindGUIWindow<UTetrisPlayEditor>("Network");
		if (nullptr == ServerEditor)
		{
			ServerEditor = UEngineGUI::CreateGUIWindow<UTetrisPlayEditor>("Network");
		}

		ServerEditor->SetActive(true);

	}
}

