#pragma once
#include <EngineCore/Actor.h>

// 설명 : 주인공 캐릭터는 무조건 상속 받아야 한다.
class APawn : public AActor
{
public:
	// constrcuter destructer
	ENGINEAPI APawn();
	ENGINEAPI ~APawn();

	// delete Function
	APawn(const APawn& _Other) = delete;
	APawn(APawn&& _Other) noexcept = delete;
	APawn& operator=(const APawn& _Other) = delete;
	APawn& operator=(APawn&& _Other) noexcept = delete;

protected:

private:

};

