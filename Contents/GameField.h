#pragma once
#include "ServerActor.h"

// Ό³Έν :
class AGameField : public AServerActor
{
public:
	AGameField();
	~AGameField();


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

private:
	// delete Function
	AGameField(const AGameField& _Other) = delete;
	AGameField(AGameField&& _Other) noexcept = delete;
	AGameField& operator=(const AGameField& _Other) = delete;
	AGameField& operator=(AGameField&& _Other) noexcept = delete;

};

