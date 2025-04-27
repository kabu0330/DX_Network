#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class UCollisionManager
{
public:
	// constrcuter destructer
	UCollisionManager();
	virtual ~UCollisionManager() = 0;

	// delete Function
	UCollisionManager(const UCollisionManager& _Other) = delete;
	UCollisionManager(UCollisionManager&& _Other) noexcept = delete;
	UCollisionManager& operator=(const UCollisionManager& _Other) = delete;
	UCollisionManager& operator=(UCollisionManager&& _Other) noexcept = delete;

	static void CreateCollisionProfile(AGameMode* _GameMode);
	static void LinkCollision(AGameMode* _GameMode);
protected:

private:

};

