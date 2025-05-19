#include "PreCompile.h"
#include "EngineDispatcher.h"

UEngineDispatcher::UEngineDispatcher()
{
}

UEngineDispatcher::~UEngineDispatcher()
{
}

void UEngineDispatcher::Release()
{
	ConvertPacketHandler.clear();
	PacketHandler.clear();
}

