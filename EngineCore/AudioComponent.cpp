#include "PreCompile.h"
#include "AudioComponent.h"

UAudioComponent::UAudioComponent()
{
}

UAudioComponent::~UAudioComponent()
{
}

void UAudioComponent::StopAllSounds()
{
	UEngineSoundManager::AllSoundStop();
}

void UAudioComponent::PauseAllSounds()
{
	UEngineSoundManager::AllSoundOff();
}

void UAudioComponent::PlayAllSounds()
{
	UEngineSoundManager::AllSoundOn();
}

void UAudioComponent::Play(std::string_view _Name, float _Volume/* = 1.0f */, bool _bIsLoop/* = false*/)
{
	Sound = UEngineSoundManager::Play(_Name);
	SetVolume(_Volume);

	if (true == _bIsLoop)
	{
		Sound.Loop(9999);
	}
}

void UAudioComponent::SetVolume(float _Volume)
{
	Sound.SetVolume(_Volume);
	CurrentVolume = _Volume;
}

bool UAudioComponent::IsPlaying()
{
	return Sound.IsPlaying();
}

void UAudioComponent::Stop()
{
	Sound.Stop();
}

void UAudioComponent::SetLoopCount(int _Count)
{
	Sound.Loop(_Count);
}

bool UAudioComponent::SwitchPause()
{
	Sound.SwtichPause();

	bool Check = false;
	return Sound.GetControl()->getPaused(&Check);
}

void UAudioComponent::Restart()
{
	Sound.Restart();
}

unsigned int UAudioComponent::GetDuration()
{
	return Sound.GetLengthMs();
}

void UAudioComponent::FadeIn(float _Duration, float _TargetVolume)
{
	if (true == bIsFading)
	{
		std::cout << "현재 사운드 Fade가 진행 중이므로 Fade In 호출을 무시합니다." << std::endl;
		return;
	}
	
	bIsFading = true;
	FadeDuration = _Duration;
	FadeStartVolume = CurrentVolume;
	FadeTargetVolume = UEngineMath::Clamp(_TargetVolume, 0.0f, _TargetVolume);
	FadeElapsed = 0.0f;

}

void UAudioComponent::FadeOut(float _Duration, float _TargetVolume)
{
	if (true == bIsFading)
	{
		std::cout << "현재 사운드 Fade가 진행 중이므로 Fade Out 호출을 무시합니다." << std::endl;
		return;
	}

	FadeIn(_Duration, _TargetVolume);
}

void UAudioComponent::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void UAudioComponent::ComponentTick(float _DeltaTime)
{
	USceneComponent::ComponentTick(_DeltaTime);
	
	Fade(_DeltaTime);
}

void UAudioComponent::Fade(float _DeltaTime)
{
	if (false == bIsFading)
	{
		return;
	}
	if (false == Sound.IsPlaying())
	{
		InitFadeVar();
		return;
	}

	FadeElapsed += _DeltaTime;
	float Alpha = FadeElapsed / FadeDuration;

	if (1.0 <= Alpha)
	{
		if (0.01f >= FadeTargetVolume)
		{
			Stop();
		}

		bIsFading = false;
		SetVolume(FadeTargetVolume);
	}
	else
	{
		float NewVolume = FadeStartVolume + ((FadeTargetVolume - FadeStartVolume) * Alpha);
		SetVolume(NewVolume);
	}
}

void UAudioComponent::InitFadeVar()
{
	bIsFading = false;
	FadeDuration = 0.0f;
	FadeTargetVolume = 0.0f;
	FadeStartVolume = 1.0f;
	CurrentVolume = 0.0f;
	FadeElapsed = 0.0f;
}

