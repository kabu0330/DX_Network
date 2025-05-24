#pragma once
#include "ActorComponent.h"
#include <EnginePlatform/ChatManager.h>

// Ό³Έν :
class UChatComponent : public UActorComponent, public UChatManager
{
public:
	UChatComponent();
	~UChatComponent();

	void ServerSendMessage(const std::string& _Message);
	void MulticastReceiveMessage(const std::string& _Message);

protected:
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

private:


private:
	// delete Function
	UChatComponent(const UChatComponent& _Other) = delete;
	UChatComponent(UChatComponent&& _Other) noexcept = delete;
	UChatComponent& operator=(const UChatComponent& _Other) = delete;
	UChatComponent& operator=(UChatComponent&& _Other) noexcept = delete;
};

