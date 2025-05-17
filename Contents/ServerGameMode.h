#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class AServerGameMode : public AGameMode
{
public:
	AServerGameMode();
	~AServerGameMode();



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	

private:
	// delete Function
	AServerGameMode(const AServerGameMode& _Other) = delete;
	AServerGameMode(AServerGameMode&& _Other) noexcept = delete;
	AServerGameMode& operator=(const AServerGameMode& _Other) = delete;
	AServerGameMode& operator=(AServerGameMode&& _Other) noexcept = delete;
};

