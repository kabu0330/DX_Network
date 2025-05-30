#pragma once
#include <map>

#include <EnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

#include "math.h"

class USoundPlayer 
{
public:
	friend class UEngineSoundManager;

	ENGINEAPI USoundPlayer();
	ENGINEAPI ~USoundPlayer();

	ENGINEAPI void On();

	ENGINEAPI void Off();
	ENGINEAPI void Stop();
	ENGINEAPI bool IsPlaying();
	ENGINEAPI void SetVolume(float _Volume);
	ENGINEAPI void SwtichPause();
	ENGINEAPI void SetPosition(unsigned int _Value);
	ENGINEAPI void Loop(int Count = -1);
	ENGINEAPI void Restart();
	ENGINEAPI unsigned int GetLengthMs();

	ENGINEAPI bool GetPause();


private:
	// 채널이 사운드 재생권한 소유
	FMOD::Channel* Control = nullptr;
	FMOD::Sound* SoundHandle = nullptr;;
};

// 설명 : FMOD 사운드 관리 클래스
class UEngineSoundManager 
{
public:
	ENGINEAPI UEngineSoundManager();
	ENGINEAPI ~UEngineSoundManager();

	ENGINEAPI static void LoadSound(std::string_view _Path);
	ENGINEAPI static void LoadSound(std::string_view _Name, std::string_view _Path);
	ENGINEAPI static USoundPlayer Play(std::string_view _Name);
	ENGINEAPI static UEngineSoundManager* Find(std::string_view _Name);
	ENGINEAPI static void Release();

	ENGINEAPI static void Update();

	ENGINEAPI static void AllSoundStop();
	ENGINEAPI static void AllSoundOff();
	ENGINEAPI static void AllSoundOn();
	ENGINEAPI static void Init();

protected:

private:
	ENGINEAPI static std::map<std::string, UEngineSoundManager*> Sounds;
	ENGINEAPI static std::list<USoundPlayer> Players;

	FMOD::Sound* SoundHandle;

	ENGINEAPI bool ResLoad(std::string_view _Path);

private:
	// delete Function
	UEngineSoundManager(const UEngineSoundManager& _Other) = delete;
	UEngineSoundManager(UEngineSoundManager&& _Other) noexcept = delete;
	UEngineSoundManager& operator=(const UEngineSoundManager& _Other) = delete;
	UEngineSoundManager& operator=(UEngineSoundManager&& _Other) noexcept = delete;
};

