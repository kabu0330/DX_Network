#include "PreCompile.h"
#include "ChatComponent.h"

UChatComponent::UChatComponent()
{
}

UChatComponent::~UChatComponent()
{
}

void UChatComponent::ServerSendMessage(const std::string& _Message)
{
}

void UChatComponent::MulticastReceiveMessage(const std::string& _Message)
{
}

void UChatComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

}

void UChatComponent::ComponentTick(float _DeltaTime)
{
	UActorComponent::ComponentTick(_DeltaTime);

}

