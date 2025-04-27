#pragma once
#include "MapObject.h"

// Ό³Έν :
class ABackground : public AMapObject
{
public:
	// constrcuter destructer
	ABackground();
	~ABackground();

	// delete Function
	ABackground(const ABackground& _Other) = delete;
	ABackground(ABackground&& _Other) noexcept = delete;
	ABackground& operator=(const ABackground& _Other) = delete;
	ABackground& operator=(ABackground&& _Other) noexcept = delete;

	std::shared_ptr<class USpriteRenderer> GetRenderer()
	{
		return Renderer;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Serialize(UEngineSerializer& _Ser) override;
	void DeSerialize(UEngineSerializer& _Ser) override;

private:
	std::shared_ptr<class UTimeEventComponent> TimeEventComponent;

	//std::shared_ptr<class USpriteRenderer> Renderer;

	std::shared_ptr<class UCollision> Collision;
};

