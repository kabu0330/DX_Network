#pragma once
#include "EngineResourceManager.h"

// Ό³Έν :
class UEngineDepthStencilState : public UEngineResourceManager
{
public:
	UEngineDepthStencilState();
	~UEngineDepthStencilState();

	static std::shared_ptr<UEngineDepthStencilState> Create(std::string_view _Name, const D3D11_DEPTH_STENCIL_DESC& _Value);

	void OMSetDepthStencilState();

protected:
	void CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState = nullptr;

private:
	// delete Function
	UEngineDepthStencilState(const UEngineDepthStencilState& _Other) = delete;
	UEngineDepthStencilState(UEngineDepthStencilState&& _Other) noexcept = delete;
	UEngineDepthStencilState& operator=(const UEngineDepthStencilState& _Other) = delete;
	UEngineDepthStencilState& operator=(UEngineDepthStencilState&& _Other) noexcept = delete;
};

