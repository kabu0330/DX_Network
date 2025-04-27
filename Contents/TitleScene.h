#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ATitleScene : public AActor
{
public:
	// constrcuter destructer
	ATitleScene();
	~ATitleScene();

	// delete Function
	ATitleScene(const ATitleScene& _Other) = delete;
	ATitleScene(ATitleScene&& _Other) noexcept = delete;
	ATitleScene& operator=(const ATitleScene& _Other) = delete;
	ATitleScene& operator=(ATitleScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;


protected:

private:
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;
	std::shared_ptr<class UContentsRenderer> TitleLogo;
	std::shared_ptr<class UContentsRenderer> CreditsLogo;
	std::shared_ptr<class UContentsRenderer> StartLogo;

	void CreateBlackParticle(float _DeltaTime);
	float TimeElapsed = 0.0f;
	float Cooldown = 0.2f;
};

