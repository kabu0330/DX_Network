#include "PreCompile.h"
#include "EngineSound.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

std::map<std::string, UEngineSoundManager*> UEngineSoundManager::Sounds;
std::list<USoundPlayer> UEngineSoundManager::Players;

// ���带 �����ϱ����� �ڵ�
FMOD::System* SoundSystem = nullptr;

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

USoundPlayer::USoundPlayer()
{
}
USoundPlayer::~USoundPlayer()
{
}

void USoundPlayer::On()
{
	Control->setPaused(false);
}

void USoundPlayer::Off()
{
	Control->setPaused(true);
}

void USoundPlayer::Stop()
{
	Control->stop();
	Control = nullptr;
}

bool USoundPlayer::IsPlaying()
{
	bool Check = true;
	Control->isPlaying(&Check);
	return Check;
}

void USoundPlayer::SetVolume(float _Volume)
{
	_Volume = UEngineMath::Clamp(_Volume, 0.0f, 1.0f);

	Control->setVolume(_Volume);
}

void USoundPlayer::SwtichPause()
{
	bool Check = false;
	Control->getPaused(&Check);

	if (true == Check)
	{
		Control->setPaused(false);
	}
	else
	{
		Control->setPaused(true);
	}
}

void USoundPlayer::SetPosition(unsigned int _Value)
{
	Control->setPosition(_Value, FMOD_TIMEUNIT_MS);
}

void USoundPlayer::Loop(int Count/* = -1*/)
{
	Control->setLoopCount(Count);
}

void USoundPlayer::Restart()
{
	SetPosition(0);
}

unsigned int USoundPlayer::GetLengthMs()
{
	unsigned int ResultLength = 0;
	SoundHandle->getLength(&ResultLength, FMOD_TIMEUNIT_MS);
	return ResultLength;
}

void UEngineSoundManager::Init()
{
	if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
	{
		MSGASSERT("FMOD �ý��� �̴ֿ� �����߽��ϴ�.");
		return;
	}

	// ���� ä�μ���
	// int maxchannels, ���ÿ� ����� ���� ����� �Ǵ°�?
	// FMOD_INITFLAGS flags, ���������� �ֳ��ε�
	// void* extradriverdata �������׿� ���� �����ͳ־��ٰ� �ֳ�.
	if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
	{
		MSGASSERT("FMOD �ý��� �̴ֿ� �����߽��ϴ�.");
		return;
	}
}

void UEngineSoundManager::AllSoundStop()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.Stop();
	}
}

void UEngineSoundManager::AllSoundOn()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.On();
	}
}

void UEngineSoundManager::AllSoundOff()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.Off();
	}
}

void UEngineSoundManager::Update()
{
	if (nullptr == SoundSystem)
	{
		return;
	}

	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; )
	{
		USoundPlayer& CurSoundPlayer = *StartIter;

		if (true == CurSoundPlayer.IsPlaying())
		{
			++StartIter;
			continue;
		}

		StartIter = Players.erase(StartIter);
		
	}

	if (FMOD_RESULT::FMOD_OK != SoundSystem->update())
	{
		MSGASSERT("FMOD �ý��� ������Ʈ�� �̻��� �����Ǿ����ϴ�.");
	}
}

// ������ ������ ���� ȣ��
void UEngineSoundManager::Release()
{
	std::map<std::string, UEngineSoundManager*>::iterator StartIter = Sounds.begin();
	std::map<std::string, UEngineSoundManager*>::iterator EndIter = Sounds.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Sounds.clear();

	if (nullptr != SoundSystem)
	{
		SoundSystem->release();
		SoundSystem = nullptr;
	}
}

UEngineSoundManager::UEngineSoundManager()
{
}

UEngineSoundManager::~UEngineSoundManager()
{
	if (nullptr != SoundHandle)
	{
		SoundHandle->release();
		SoundHandle = nullptr;
	}
}

void UEngineSoundManager::LoadSound(std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();

	UEngineSoundManager::LoadSound(FileName.data(), _Path);
}

void UEngineSoundManager::LoadSound(std::string_view _Name, std::string_view _Path)
{
	// �̳༮�� UTF-8�� ��θ� �ٲ���� �Ҽ� �ִ�.
	std::string UpperString = UEngineString::ToUpper(_Name);

	UEngineSoundManager* NewSound = new UEngineSoundManager();

	if (false != UEngineSoundManager::Sounds.contains(UpperString))
	{
		delete NewSound;
		MSGASSERT("�̹� �ε��� ���带 �� �ε��Ϸ��� �߽��ϴ�." + UpperString);
		return;
	}

	if (false == NewSound->ResLoad(_Path))
	{
		delete NewSound;
		MSGASSERT("���� �ε忡 �����߽��ϴ�" + UpperString);
		return;
	}

	UEngineSoundManager::Sounds.insert({ UpperString, NewSound });
	// Load(FileName, Path);
}

UEngineSoundManager* UEngineSoundManager::Find(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == Sounds.contains(UpperString))
	{
		return nullptr;
	}

	return Sounds[UpperString];
}

USoundPlayer UEngineSoundManager::Play(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	UEngineSoundManager* FindSound = Find(_Name);

	if (nullptr == FindSound)
	{
		MSGASSERT("�ε����� ���� ���带 ����Ϸ��� �߽��ϴ�" + UpperString);
	}


	// �׳� �ܼ��� ����ϴ°� �ƴ϶�� ä���� ���;� �ӷ��̳� ��ġ ���� �ͽ� ����� �����Ҽ� �ִ�.
	FMOD::Channel* Ch = nullptr;

	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Ch);

	// 1�� ���
	Ch->setLoopCount(0);

	// ���� 1��
	Ch->setVolume(1.0f);

	USoundPlayer NewPlayer;
	NewPlayer.Control = Ch;
	NewPlayer.SoundHandle = FindSound->SoundHandle;

	Players.push_back(NewPlayer);

	return NewPlayer;
}


bool UEngineSoundManager::ResLoad(std::string_view _Path)
{
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);

	if (nullptr == SoundHandle)
	{
		MSGASSERT("���� �ε��� �����߽��ϴ�" + std::string(_Path));
		return false;
	}

	return true;
}