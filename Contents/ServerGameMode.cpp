#include "PreCompile.h"
#include "ServerGameMode.h"
#include <EngineCore/EngineGUI.h>

AServerGameMode::AServerGameMode()
{
}

AServerGameMode::~AServerGameMode()
{
}

void AServerGameMode::BeginPlay()
{
	AGameMode::BeginPlay();
}

void AServerGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);
}

void AServerGameMode::LevelChangeStart()
{
	UEngineGUI::AllWindowOff();
	{
		ServerEditor = UEngineGUI::FindGUIWindow<UServerEditor>(UEngineString::AnsiToUTF8("��Ʈ��ũ_������"));
		if (nullptr == ServerEditor)
		{
			ServerEditor = UEngineGUI::CreateGUIWindow<UServerEditor>(UEngineString::AnsiToUTF8("��Ʈ��ũ_������"));
		}

		ServerEditor->SetActive(true);
	}
}

