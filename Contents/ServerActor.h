#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/NetObject.h>

// Ό³Έν :
class AServerActor : public AActor, public UNetObject
{
public:
	AServerActor();
	~AServerActor();



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	USpriteRenderer* BodyRenderer = nullptr;

private:

private:
	// delete Function
	AServerActor(const AServerActor& _Other) = delete;
	AServerActor(AServerActor&& _Other) noexcept = delete;
	AServerActor& operator=(const AServerActor& _Other) = delete;
	AServerActor& operator=(AServerActor&& _Other) noexcept = delete;
};

