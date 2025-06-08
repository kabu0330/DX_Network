#pragma once
#include "ContentEnum.h"
#include "ServerActor.h"

// Ό³Έν :
class UGlobalData
{
public:
	virtual ~UGlobalData() = 0;

	static AServerActor* GetSpawnActor(int _Type, ULevel* _World);

protected:

private:

private:
	UGlobalData();

	// delete Function
	UGlobalData(const UGlobalData& _Other) = delete;
	UGlobalData(UGlobalData&& _Other) noexcept = delete;
	UGlobalData& operator=(const UGlobalData& _Other) = delete;
	UGlobalData& operator=(UGlobalData&& _Other) noexcept = delete;
};

