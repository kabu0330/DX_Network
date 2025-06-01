#pragma once
#include "ServerPawn.h"

// Ό³Έν :
class ATetromino : public AServerPawn
{
public:
	ATetromino();
	~ATetromino();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

	void SetControllMode(float _DeltaTime) override;

private:


private:
	// delete Function
	ATetromino(const ATetromino& _Other) = delete;
	ATetromino(ATetromino&& _Other) noexcept = delete;
	ATetromino& operator=(const ATetromino& _Other) = delete;
	ATetromino& operator=(ATetromino&& _Other) noexcept = delete;
};

