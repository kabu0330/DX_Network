#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class APostEffectGameMode : public AGameMode
{
public:
	// construter destructer
	APostEffectGameMode();
	~APostEffectGameMode();

	// delete Function
	APostEffectGameMode(const APostEffectGameMode& _Other) = delete;
	APostEffectGameMode(APostEffectGameMode&& _Other) noexcept = delete;
	APostEffectGameMode& operator=(const APostEffectGameMode& _Other) = delete;
	APostEffectGameMode& operator=(APostEffectGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


private:
	class ACameraActor* Camera = nullptr;
};

