#pragma once
#include <EngineCore/Pawn.h>
#include <EngineCore/>

// Ό³Έν :
class ServerPawn : public APawn, public 
{
public:
	// construter destructer
	ServerPawn();
	~ServerPawn();

	// delete Function
	ServerPawn(const ServerPawn& _Other) = delete;
	ServerPawn(ServerPawn&& _Other) noexcept = delete;
	ServerPawn& operator=(const ServerPawn& _Other) = delete;
	ServerPawn& operator=(ServerPawn&& _Other) noexcept = delete;

protected:

private:

};

