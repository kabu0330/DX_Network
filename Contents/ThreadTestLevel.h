#pragma once
#include <EnginePlatform/EngineThread.h>
#include <EngineCore/Actor.h>

// Ό³Έν :
class ThreadTestLevel : public AActor
{
public:
	// construter destructer
	ThreadTestLevel();
	~ThreadTestLevel();

	// delete Function
	ThreadTestLevel(const ThreadTestLevel& _Other) = delete;
	ThreadTestLevel(ThreadTestLevel&& _Other) noexcept = delete;
	ThreadTestLevel& operator=(const ThreadTestLevel& _Other) = delete;
	ThreadTestLevel& operator=(ThreadTestLevel&& _Other) noexcept = delete;

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

