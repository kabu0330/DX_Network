#include "PreCompile.h"
#include "FSMComponent.h"

UFSMComponent::UFSMComponent()
{
}

UFSMComponent::~UFSMComponent()
{
}

void UFSMComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}

void UFSMComponent::ComponentTick(float _DeltaTime)
{
	UActorComponent::ComponentTick(_DeltaTime);

	if (false == GetActor()->IsActive())
	{
		return;
	}
	FSM.Update(_DeltaTime);
}

