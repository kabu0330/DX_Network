#include "PreCompile.h"
#include "ServerGameMode.h"

AServerGameMode::AServerGameMode()
{
}

AServerGameMode::~AServerGameMode()
{
}

void AServerGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AServerGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);
}

