#include "PreCompile.h"
#include "ActorComponent.h"
#include "Actor.h"

UActorComponent::UActorComponent()
{
}

UActorComponent::~UActorComponent()
{
}

bool UActorComponent::IsActive()
{
	if (nullptr == GetActor())
	{
		MSGASSERT("액터 컴포넌트에 부모가 설정되어있지 않습니다.");
		return false;
	}

	return UObject::IsActive() && GetActor()->IsActive();
}

bool UActorComponent::IsDestroy()
{
	return UObject::IsDestroy() || GetActor()->IsDestroy();
}

class AActor* UActorComponent::GetActor()
{
	return Actor;
}

ULevel* UActorComponent::GetWorld()
{
	return Actor->GetWorld();
}
