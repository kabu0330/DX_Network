#pragma once
#include "EngineNetwork.h"

// Ό³Έν :
class UEngineServer : public UEngineNetwork
{
public:
	UEngineServer();
	~UEngineServer();

	ENGINEAPI int CreateObjectToken()
	{
		return ObjectTokenCreator++;
	}
	ENGINEAPI int CreateSessionToken()
	{
		return SessionTokenCreator++;
	}


protected:
	std::atomic<int> SessionTokenCreator;
	std::atomic<int> ObjectTokenCreator;


private:
	// delete Function
	UEngineServer(const UEngineServer& _Other) = delete;
	UEngineServer(UEngineServer&& _Other) noexcept = delete;
	UEngineServer& operator=(const UEngineServer& _Other) = delete;
	UEngineServer& operator=(UEngineServer&& _Other) noexcept = delete;
};

