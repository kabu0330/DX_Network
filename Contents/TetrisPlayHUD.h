#pragma once
#include <EngineCore/HUD.h>

// Ό³Έν :
class ATetrisPlayHUD : public AHUD
{
public:
	ATetrisPlayHUD();
	~ATetrisPlayHUD();



protected:

private:
	// delete Function
	ATetrisPlayHUD(const ATetrisPlayHUD& _Other) = delete;
	ATetrisPlayHUD(ATetrisPlayHUD&& _Other) noexcept = delete;
	ATetrisPlayHUD& operator=(const ATetrisPlayHUD& _Other) = delete;
	ATetrisPlayHUD& operator=(ATetrisPlayHUD&& _Other) noexcept = delete;
};

