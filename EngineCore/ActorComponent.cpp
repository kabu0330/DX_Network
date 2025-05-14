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
		MSGASSERT("���� ������Ʈ�� �θ� �����Ǿ����� �ʽ��ϴ�.");
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
