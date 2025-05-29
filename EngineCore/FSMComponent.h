#pragma once
#include <EngineBase/FSMStateManager.h>
#include "ActorComponent.h"

// Ό³Έν :
class UFSMComponent : public UActorComponent
{
public:
	UFSMComponent();
	~UFSMComponent();

	UFSMStateManager& GetFSM()
	{
		return FSM;
	}

protected:
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

private:
	UFSMStateManager FSM = UFSMStateManager();

private:
	// delete Function
	UFSMComponent(const UFSMComponent& _Other) = delete;
	UFSMComponent(UFSMComponent&& _Other) noexcept = delete;
	UFSMComponent& operator=(const UFSMComponent& _Other) = delete;
	UFSMComponent& operator=(UFSMComponent&& _Other) noexcept = delete;
};

