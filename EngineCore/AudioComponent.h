#pragma once
#include <EnginePlatform/EngineSound.h>
#include "SceneComponent.h"

// 설명 :
class UAudioComponent : public USceneComponent
{
public:
	UAudioComponent();
	~UAudioComponent();

	static void StopAllSounds();

	static void PauseAllSounds();
	static void PlayAllSounds();


	void Play(std::string_view _Name, float _Volume = 1.0f, bool _IsLoop = false);
	bool IsPlaying();

	void Stop(); // 사운드 정보 삭제

	void SetVolume(float _Volume);
	void SetLoopCount(int _Count);
	
	// Off : true, On : false
	bool SwitchPause();

	void Restart();
	unsigned int GetDuration();
	
	void FadeIn(float _Duration, float _TargetVolume);
	void FadeOut(float _Duration, float _TargetVolume);

protected:
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

private:
	USoundPlayer Sound = USoundPlayer();
	
	bool bIsFading = false;
	float FadeDuration = 0.0f;
	float FadeTargetVolume = 0.0f;
	float FadeStartVolume = 1.0f;
	float CurrentVolume = 0.0f;
	float FadeElapsed = 0.0f;
	void Fade(float _DeltaTime);
	void InitFadeVar();

private:
	// delete Function
	UAudioComponent(const UAudioComponent& _Other) = delete;
	UAudioComponent(UAudioComponent&& _Other) noexcept = delete;
	UAudioComponent& operator=(const UAudioComponent& _Other) = delete;
	UAudioComponent& operator=(UAudioComponent&& _Other) noexcept = delete;

};

