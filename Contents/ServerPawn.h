#pragma once
#include <EngineCore/Pawn.h>
#include <EngineCore/NetObject.h>

// Ό³Έν :
class AServerPawn : public APawn, public UNetObject
{
public:
	AServerPawn();
	~AServerPawn();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetNetMode(float _DeltaTime);
	virtual void SetControllMode(float _DeltaTime);

	float CurFramePacketTime = 0.0f;
	std::shared_ptr<class UTimeEventComponent> TimeEventComponent = nullptr;

private:

private:
	// delete Function
	AServerPawn(const AServerPawn& _Other) = delete;
	AServerPawn(AServerPawn&& _Other) noexcept = delete;
	AServerPawn& operator=(const AServerPawn& _Other) = delete;
	AServerPawn& operator=(AServerPawn&& _Other) noexcept = delete;

};

