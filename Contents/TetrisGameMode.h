#pragma once
#include <EngineCore/GameMode.h>
#include "ServerEditor.h"

// Ό³Έν :
class TetrisGameMode : public AGameMode
{
public:
	TetrisGameMode();
	~TetrisGameMode();



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	std::shared_ptr<UServerEditor> ServerEditor = nullptr;

private:
	// delete Function
	TetrisGameMode(const TetrisGameMode& _Other) = delete;
	TetrisGameMode(TetrisGameMode&& _Other) noexcept = delete;
	TetrisGameMode& operator=(const TetrisGameMode& _Other) = delete;
	TetrisGameMode& operator=(TetrisGameMode&& _Other) noexcept = delete;
};

