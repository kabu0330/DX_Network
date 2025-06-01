#pragma once
#include <EngineCore/GameMode.h>
#include "ServerEditor.h"

// Ό³Έν :
class ATetrisGameMode : public AGameMode
{
public:
	ATetrisGameMode();
	~ATetrisGameMode();



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	std::shared_ptr<UEngineGUIWindow> ServerEditor = nullptr;

private:
	// delete Function
	ATetrisGameMode(const ATetrisGameMode& _Other) = delete;
	ATetrisGameMode(ATetrisGameMode&& _Other) noexcept = delete;
	ATetrisGameMode& operator=(const ATetrisGameMode& _Other) = delete;
	ATetrisGameMode& operator=(ATetrisGameMode&& _Other) noexcept = delete;
};

