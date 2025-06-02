#pragma once
#include "SceneComponent.h"
#include <EngineCore/EngineCore.h>

// 설명 :
// 언리얼에서 Actor는 절대 트랜스폼을 가지지 않는다.
class AActor : public UObject
{
	friend class ULevel;
	friend class APlayerController;

public:
	ENGINEAPI AActor();
	ENGINEAPI ~AActor();

	ENGINEAPI virtual void BeginPlay();
	ENGINEAPI virtual void Tick(float _DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	template<typename ComponentType>
	inline std::shared_ptr<ComponentType> CreateDefaultSubObject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");
			return nullptr;
		}

		size_t Size = sizeof(ComponentType);

		char* ComMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComPtr = reinterpret_cast<ComponentType*>(ComMemory);
		ComPtr->Actor = this;

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComMemory);
		// 레벨먼저 세팅하고
		// 플레이스먼트 new 
		std::shared_ptr<ComponentType> NewCom(new(ComMemory) ComponentType());

		AllComponentList.push_back(NewCom);

		// 내가 그냥 ActorComponent
		// 내가 그냥 SceneComponent
		if (std::is_base_of_v<UActorComponent, ComponentType> 
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewCom);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("말도 안됨");
		}

		return NewCom;
	}

	template<typename Type>
	Type* GetGameInstance()
	{
		return dynamic_cast<Type*>(GetGameInstance());
	}

	ENGINEAPI class UGameInstance* GetGameInstance();

	ULevel* GetWorld()
	{
		return World;
	}

	ENGINEAPI void AttachToActor(AActor* _Parent);

	FTransform GetActorTransform()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FTransform();
		}

		return RootComponent->GetTransformRef();
	}

	void SetActorTransform(const FTransform& _Transform)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->Transform = _Transform;

		return;
	}

	class USceneComponent* GetRootComponent()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return nullptr;
		}
		return RootComponent.get();
	}



	// Scale
	// World
	FVector GetActorWorldScale3D()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FVector();
		}
		return RootComponent->GetWorldScale3D();
	}
	void SetActorWorldScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->SetScale3D(_Scale);
	}

	// Local
	void SetActorRelativeScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->SetRelativeScale3D(_Scale);
	}
	FVector GetActorRelativeScale3D()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FVector();
		}
		return RootComponent->GetRelativeScale3D();
	}


	// Rotation
	// World
	FVector GetActorRotation()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FVector();
		}
		
		return RootComponent->Transform.Rotation;
	}
	void SetActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->SetLocalRotation(_Value);
	}
	void AddActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->AddWorldRotation(_Value);
	}


	// Location
    // World
	FVector GetActorLocation()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FVector();
		}

		return RootComponent->Transform.WorldLocation;
	}
	void SetActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->SetWorldLocation(_Value);
	}
	void AddActorWorldOffset(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->AddWorldLocation(_Value);
	}

	// Local
	FVector GetActorRelativeLocation()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return FVector();
		}

		return RootComponent->Transform.RelativeLocation;
	}
	void SetActorRelativeLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->SetRelativeLocation(_Value);
	}
	void AddActorLocalOffset(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return;
		}

		RootComponent->AddRelativeLocation(_Value);
	}



	ENGINEAPI FVector GetActorUpVector();
	ENGINEAPI FVector GetActorRightVector();
	ENGINEAPI FVector GetActorForwardVector();

	template<typename ComType>
	std::vector<std::shared_ptr<ComType>> GetComponentByClass()
	{
		std::vector<std::shared_ptr<ComType>> Result;

		for (std::shared_ptr<class UActorComponent> Component : AllComponentList)
		{
			std::shared_ptr<ComType> Com = std::dynamic_pointer_cast<ComType>(Component);
			if (nullptr != Com)
			{
				Result.push_back(Com);
			}
		}

		return Result;
	}

	class APlayerController* GetPlayerController()
	{
		if (nullptr == PlayerController)
		{
			MSGASSERT("SetupAttachment()로 RootComponent를 설정해주세요.");
			return nullptr;
		}
		return PlayerController;
	}

protected:
	std::shared_ptr<class USceneComponent> RootComponent = nullptr;


private:
	void SetOwner(class APlayerController* _PlayerController)
	{
		PlayerController = _PlayerController;
	}
	
	AActor* Parent = nullptr;
	std::list<std::shared_ptr<AActor>> ChildList;
	class APlayerController* PlayerController = nullptr;

	ULevel* World; 	// 스폰액터 방식이 변경되었으니까. 초기화 안한다.

	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;

	// 레퍼런스 카운트 유지용 자료구조.
	std::list<std::shared_ptr<class UActorComponent>> AllComponentList;

private:
	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;
};

using Super = AActor;

