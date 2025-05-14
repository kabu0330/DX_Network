#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABasicVisualActor : public AActor
{
public:
	// construter destructer
	ABasicVisualActor();
	~ABasicVisualActor();

	// delete Function
	ABasicVisualActor(const ABasicVisualActor& _Other) = delete;
	ABasicVisualActor(ABasicVisualActor&& _Other) noexcept = delete;
	ABasicVisualActor& operator=(const ABasicVisualActor& _Other) = delete;
	ABasicVisualActor& operator=(ABasicVisualActor&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* Renderer = nullptr;
};

