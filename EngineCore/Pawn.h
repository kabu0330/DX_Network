#pragma once
#include <EngineCore/Actor.h>

// ���� : ���ΰ� ĳ���ʹ� ������ ��� �޾ƾ� �Ѵ�.
class APawn : public AActor
{
public:
	ENGINEAPI APawn();
	ENGINEAPI ~APawn();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	std::shared_ptr<class USpriteRenderer> BodyRenderer = nullptr;
	std::shared_ptr<class UCollision> BodyCollision = nullptr;

private:
	// delete Function
	APawn(const APawn& _Other) = delete;
	APawn(APawn&& _Other) noexcept = delete;
	APawn& operator=(const APawn& _Other) = delete;
	APawn& operator=(APawn&& _Other) noexcept = delete;
};

