#include "PreCompile.h"
#include "TetrisGameMode.h"
#include "TetrisPlayEditor.h"

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
		ServerEditor = UEngineGUI::FindGUIWindow<UTetrisPlayEditor>(UEngineString::AnsiToUTF8("��Ʈ��ũ_������"));
		if (nullptr == ServerEditor)
		{
			ServerEditor = UEngineGUI::CreateGUIWindow<UTetrisPlayEditor>(UEngineString::AnsiToUTF8("��Ʈ��ũ_������"));
		}

		ServerEditor->SetActive(true);
	}
}

