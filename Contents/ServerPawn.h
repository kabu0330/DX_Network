#pragma once
#include <EngineCore/Pawn.h>
#include <EngineCore/NetObject.h>

// Ό³Έν :
class ServerPawn : public APawn, public UNetObject
{
public:
	ServerPawn();
	~ServerPawn();


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<class UTimeEventComponent> TimeEventComponent = nullptr;
	std::shared_ptr<class USpriteRenderer> Renderer = nullptr;
	std::shared_ptr<class UCollision> Collision = nullptr;
	float CurFramePacketTime = 0.0f;

	void SetNetMode(float _DeltaTime);
	void SetControllMode(float _DeltaTime);

private:
	// delete Function
	ServerPawn(const ServerPawn& _Other) = delete;
	ServerPawn(ServerPawn&& _Other) noexcept = delete;
	ServerPawn& operator=(const ServerPawn& _Other) = delete;
	ServerPawn& operator=(ServerPawn&& _Other) noexcept = delete;

};

