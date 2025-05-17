#include "PreCompile.h"
#include "ServerPawn.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/PlayerController.h>

ServerPawn::ServerPawn()
{
	CurFramePacketTime = 20.0f * 30.0f;
}

ServerPawn::~ServerPawn()
{
}

void ServerPawn::BeginPlay()
{
}

void ServerPawn::Tick(float _DeltaTime)
{
}

void ServerPawn::SetNetMode(float _DeltaTime)
{
}

void ServerPawn::SetControllMode(float _DeltaTime)
{
}

