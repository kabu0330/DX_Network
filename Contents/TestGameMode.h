#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineThread.h>

// Ό³Έν :
class ATestGameMode : public AGameMode
{
public:
	// construter destructer
	ATestGameMode();
	~ATestGameMode();

	// delete Function
	ATestGameMode(const ATestGameMode& _Other) = delete;
	ATestGameMode(ATestGameMode&& _Other) noexcept = delete;
	ATestGameMode& operator=(const ATestGameMode& _Other) = delete;
	ATestGameMode& operator=(ATestGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelChangeStart() override;


private:
	class ACameraActor* Camera = nullptr;

	UEngineThread Thread;
};

