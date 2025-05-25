#pragma once
#include "Renderer.h"

// Ό³Έν :
class UGridRenderer : public URenderer
{
public:
	UGridRenderer();
	~UGridRenderer();



protected:

private:


private:
	// delete Function
	UGridRenderer(const UGridRenderer& _Other) = delete;
	UGridRenderer(UGridRenderer&& _Other) noexcept = delete;
	UGridRenderer& operator=(const UGridRenderer& _Other) = delete;
	UGridRenderer& operator=(UGridRenderer&& _Other) noexcept = delete;
};

