#pragma once
#include "SceneComponent.h"
#include <EngineCore/EngineCore.h>

// ���� :
// �𸮾󿡼� Actor�� ���� Ʈ�������� ������ �ʴ´�.
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
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �߽��ϴ�.");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �߽��ϴ�.");
			return nullptr;
		}

		size_t Size = sizeof(ComponentType);

		char* ComMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComPtr = reinterpret_cast<ComponentType*>(ComMemory);
		ComPtr->Actor = this;

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComMemory);
		// �������� �����ϰ�
		// �÷��̽���Ʈ new 
		std::shared_ptr<ComponentType> NewCom(new(ComMemory) ComponentType());

		AllComponentList.push_back(NewCom);

		// ���� �׳� ActorComponent
		// ���� �׳� SceneComponent
		if (std::is_base_of_v<UActorComponent, ComponentType> 
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewCom);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("���� �ȵ�");
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
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return FTransform();
		}

		return RootComponent->GetTransformRef();
	}

	void SetActorTransform(const FTransform& _Transform)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->Transform = _Transform;

		return;
	}

	class USceneComponent* GetRootComponent()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
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
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return FVector();
		}
		return RootComponent->GetWorldScale3D();
	}
	void SetActorWorldScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->SetScale3D(_Scale);
	}

	// Local
	void SetActorRelativeScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->SetRelativeScale3D(_Scale);
	}
	FVector GetActorRelativeScale3D()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
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
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return FVector();
		}
		
		return RootComponent->Transform.Rotation;
	}
	void SetActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->SetLocalRotation(_Value);
	}
	void AddActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
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
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return FVector();
		}

		return RootComponent->Transform.WorldLocation;
	}
	void SetActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->SetWorldLocation(_Value);
	}
	void AddActorWorldOffset(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->AddWorldLocation(_Value);
	}

	// Local
	FVector GetActorRelativeLocation()
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return FVector();
		}

		return RootComponent->Transform.RelativeLocation;
	}
	void SetActorRelativeLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
			return;
		}

		RootComponent->SetRelativeLocation(_Value);
	}
	void AddActorLocalOffset(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
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
			MSGASSERT("SetupAttachment()�� RootComponent�� �������ּ���.");
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

	ULevel* World; 	// �������� ����� ����Ǿ����ϱ�. �ʱ�ȭ ���Ѵ�.

	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;

	// ���۷��� ī��Ʈ ������ �ڷᱸ��.
	std::list<std::shared_ptr<class UActorComponent>> AllComponentList;

private:
	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;
};

using Super = AActor;

