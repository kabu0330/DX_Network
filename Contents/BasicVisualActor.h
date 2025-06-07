#pragma once
#include <EngineCore/Actor.h>

// ���� :
class ABasicVisualActor : public AActor
{
public:
	ABasicVisualActor();
	~ABasicVisualActor();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	class USpriteRenderer* Renderer = nullptr;

private:
	// delete Function
	ABasicVisualActor(const ABasicVisualActor& _Other) = delete;
	ABasicVisualActor(ABasicVisualActor&& _Other) noexcept = delete;
	ABasicVisualActor& operator=(const ABasicVisualActor& _Other) = delete;
	ABasicVisualActor& operator=(ABasicVisualActor&& _Other) noexcept = delete;
};

