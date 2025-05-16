#pragma once
#include <EngineCore/EngineRenderTarget.h>

// Ό³Έν :
class UTestPostEffect : public UPostEffect
{
public:
	UTestPostEffect();
	~UTestPostEffect();

protected:
	void Init() override;
	void Effect(UEngineCamera* _Camera, float _Deltatime) override;

private:
	std::shared_ptr<UEngineRenderTarget> CopyTarget;

private:
	// delete Function
	UTestPostEffect(const UTestPostEffect& _Other) = delete;
	UTestPostEffect(UTestPostEffect&& _Other) noexcept = delete;
	UTestPostEffect& operator=(const UTestPostEffect& _Other) = delete;
	UTestPostEffect& operator=(UTestPostEffect&& _Other) noexcept = delete;

};

