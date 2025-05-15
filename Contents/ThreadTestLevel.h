#pragma once
#include <EnginePlatform/EngineThread.h>
#include <EngineCore/GameMode.h>

// Ό³Έν :
class AThreadTestLevel : public AGameMode
{
public:
	// construter destructer
	AThreadTestLevel();
	~AThreadTestLevel();

	// delete Function
	AThreadTestLevel(const AThreadTestLevel& _Other) = delete;
	AThreadTestLevel(AThreadTestLevel&& _Other) noexcept = delete;
	AThreadTestLevel& operator=(const AThreadTestLevel& _Other) = delete;
	AThreadTestLevel& operator=(AThreadTestLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelChangeStart() override;

private:

	std::atomic<int> LoadingCount = 0;

	std::atomic<bool> ThreadLoadingInit = false;

	std::atomic<bool> ThreadLoadingEnd = false;

	UEngineThread Thread;
};

