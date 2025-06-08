#include "PreCompile.h"
#include "GlobalData.h"
#include "Tetromino.h"
#include "GameField.h"

UGlobalData::UGlobalData()
{
}

UGlobalData::~UGlobalData()
{
}

AServerActor* UGlobalData::GetSpawnActor(int _Type, ULevel* _World)
{
	ETypeInfo Type = static_cast<ETypeInfo>(_Type);
	AServerActor* NetActor = nullptr;
	switch (Type)
	{
	case ETypeInfo::AGameField:
		NetActor = _World->SpawnActor<AGameField>().get();
		break;
	default:
		break;
	}

	return NetActor;
}

