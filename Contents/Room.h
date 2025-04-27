#pragma once
#include <EngineCore/Actor.h>
#include "Monster.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EnginePlatform/EngineSound.h>

// 설명 :
class ARoom : public AActor
{
public:
	// constrcuter destructer
	ARoom();
	~ARoom();

	// delete Function
	ARoom(const ARoom& _Other) = delete;
	ARoom(ARoom&& _Other) noexcept = delete;
	ARoom& operator=(const ARoom& _Other) = delete;
	ARoom& operator=(ARoom&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UEngineWinImage& GetPixelCollisionImage()
	{
		return PixelCollisionImage;
	}
	static ARoom* GetCurRoom()
	{
		return CurRoom;
	}
	static void SetCurRoom(ARoom* _Room)
	{
		CurRoom = _Room;
	}

	// 초기 세팅
	void CreateTexture(std::string_view _FileName, float _ScaleRatio);
	void SetRoomLocation(FVector _Pos);

	class ADoor* CreateDoor(FVector _DoorScale, FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsDoor = false);

	// 중력과 벽
	void CheckPixelCollisionWithGravity(AActor* _Actor, FVector _Offset);
	void CheckPixelCollisionWithWall(AActor* _Actor, float _Speed, bool _Left, FVector _Offset);
	bool CheckPixelCollisionWithWall(AActor* _Actor, float _Speed, bool _Left); // 넉백 상황
	void CheckPixelCollisionWithCeil(AActor* _Actor, class UContentsRenderer* _Renderer, float _Speed, bool _Left, FVector _Offset);
	bool IsOnGround(FVector _Pos);
	FVector GetPixelCollisionPoint(AActor* _Actor, FVector _Offset);

	FVector GetSize() const
	{
		return Size;
	}
	void SetSize(const FVector& _Size)
	{
		Size = _Size;
	}
	void SetLeftTopPos(const FVector& _LeftTopPos)
	{
		LeftTopPos = _LeftTopPos;
	}
	FVector GetLeftTopPos()
	{
		return LeftTopPos;
	}

	static void SwitchDebugActiveGravity()
	{
		bActiveGravity = !bActiveGravity;
	}
	static bool GetActiveGravity()
	{
		return bActiveGravity;
	}
	static void SetActiveGravity(bool _bActiveGravity)
	{
		bActiveGravity = _bActiveGravity;
	}

	std::list<AMonster*>& GetMonstersRef()
	{
		return Monsters;
	}

	USoundPlayer& GetSoundRef()
	{
		return Sound;
	}

	void SetSound(std::string_view _SoundName)
	{
		SoundName = _SoundName;
	}

	template<typename MonsterType>
	AMonster* CreateMonster(FVector _Pos, bool _CanWorldPos = true)
	{
		AMonster* NewMonster = GetWorld()->SpawnActor<MonsterType>().get();
		if (false == _CanWorldPos)
		{
			NewMonster->SetActorLocation(this->GetActorLocation() + _Pos);
		}
		else
		{
			NewMonster->SetActorLocation(_Pos);
		}

		NewMonster->SetParentRoom(this);
		Monsters.push_back(NewMonster);
		return NewMonster;
	}

	template<typename MapObjectType>
	MapObjectType* CreateObject(FVector _Pos, FVector _Scale, bool _CanWorldPos = true)
	{
		MapObjectType* NewObject = GetWorld()->SpawnActor<MapObjectType>().get();
		NewObject->SetScale(_Scale);
		if (false == _CanWorldPos)
		{
			NewObject->SetActorLocation(this->GetActorLocation() + _Pos);
		}
		else
		{
			NewObject->SetActorLocation(_Pos);
		}

		return NewObject;
	}
	void SetRoomActive(bool _Value)
	{
		if (nullptr != PixelCollisionTexture)
		{
			PixelCollisionTexture->SetActive(_Value);
		}
		if (nullptr != BackgroundRenderer)
		{
			BackgroundRenderer->SetActive(_Value);
		}
		if (true != Monsters.empty())
		{
			if ("FalseKnightRoom" != GetName())
			{
				for (AMonster* Monster : Monsters)
				{
					Monster->SetActive(_Value);
				}
			}
		}
		this->SetActive(_Value);
	}

	void CheckActiveRoom()
	{
		if (this != CurRoom)
		{
			SetRoomActive(false);
		}
		else if (this == CurRoom)
		{
			SetRoomActive(true);
		}
	}

	static void SetBackgroundSound(std::string_view _SoundFile) // 보스전을 위해
	{
		Sound.Stop();
		Sound = UEngineSound::Play(_SoundFile);
		Sound.Loop(999);
	}

	class ABarrier* CreateBarrier(FVector _Pos);

	std::vector<class ABarrier*>& GetBarrier()
	{
		return Barriers;
	}


protected:
	void Force(AActor* _Actor, float _DeltaTime);
	void BossSpawn(class UCollision* _This, class UCollision* _Other);
private:
	inline static bool bActiveGravity = false;
	float GravityValue = 0.0f;

	inline static ARoom* CurRoom = nullptr;
	std::vector<class ABarrier*> Barriers;
	class UCollision* Collision = nullptr;
	UEngineWinImage PixelCollisionImage;
	FVector Size = FVector::ZERO;
	FVector LeftTopPos = FVector::ZERO;

	std::shared_ptr<class UContentsRenderer> PixelCollisionTexture;
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;
	//class UCollision* Trap = nullptr;
	class ADoor* Door = nullptr;

	std::list<AMonster*> Monsters;

	inline static USoundPlayer Sound = USoundPlayer();
	USoundPlayer SoundWind = USoundPlayer();
	std::string SoundName = "";
	inline static std::string CurSoundName = "";
	bool bIsSoundPlaying = false;
	void SoundPlay();
	
};

