#pragma once
#include "ServerActor.h"
#include <vector>

// Ό³Έν :
class AGameField : public AServerActor
{
public:
	AGameField();
	~AGameField();

	FPoint GetPoint(FVector _Pos);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class UCollision* Collision = nullptr;
	std::vector<class UCollision*> DebugCollision;

	FVector ReferencePoint = FVector::ZERO;
	
	std::vector<std::vector<int>> Point;
private:
	// delete Function
	AGameField(const AGameField& _Other) = delete;
	AGameField(AGameField&& _Other) noexcept = delete;
	AGameField& operator=(const AGameField& _Other) = delete;
	AGameField& operator=(AGameField&& _Other) noexcept = delete;

};

