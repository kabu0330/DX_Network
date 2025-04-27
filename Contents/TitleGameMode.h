#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/TimeEventComponent.h>

// Ό³Έν :
class ATitleGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATitleGameMode();
	~ATitleGameMode();

	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

	void FadeEffect();
protected:
	void InitBackgroundSound();
	void StartSound();

private:
	std::shared_ptr<class ATitleScene> TitleScene;
	UTimeEventComponent* TimeEventer = nullptr;
	USoundPlayer Sound;
	USoundPlayer ButtonSound;
	float Volume = 1.0f;
	bool bCanNextMode = false;
	bool bIsSpace = false;
	class ATitleHUD* HUD = nullptr;
};

