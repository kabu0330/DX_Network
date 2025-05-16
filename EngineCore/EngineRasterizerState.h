#pragma once
#include "EngineResourceManager.h"

// Ό³Έν :
class UEngineRasterizerState : public UEngineResourceManager
{
public:
	// constrcuter destructer
	UEngineRasterizerState();
	~UEngineRasterizerState();

	// delete Function
	UEngineRasterizerState(const UEngineRasterizerState& _Other) = delete;
	UEngineRasterizerState(UEngineRasterizerState&& _Other) noexcept = delete;
	UEngineRasterizerState& operator=(const UEngineRasterizerState& _Other) = delete;
	UEngineRasterizerState& operator=(UEngineRasterizerState&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineRasterizerState> Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value);

	void RSSetState();

protected:

private:
	void CreateRasterizerState(const D3D11_RASTERIZER_DESC& _Value);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;
};

