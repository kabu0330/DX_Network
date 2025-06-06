#pragma once
#include <EngineCore/GameMode.h>
#include "ServerEditor.h"

// ���� :
class AServerGameMode : public AGameMode
{
public:
	AServerGameMode();
	~AServerGameMode();



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	std::shared_ptr<UServerEditor> ServerEditor = nullptr;

private:
	// delete Function
	AServerGameMode(const AServerGameMode& _Other) = delete;
	AServerGameMode(AServerGameMode&& _Other) noexcept = delete;
	AServerGameMode& operator=(const AServerGameMode& _Other) = delete;
	AServerGameMode& operator=(AServerGameMode&& _Other) noexcept = delete;
};

