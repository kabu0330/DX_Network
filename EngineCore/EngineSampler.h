#pragma once
#include "EngineResources.h"
#include "EngineEnums.h"

// Ό³Έν :
class UEngineSampler : public UEngineResources
{
public:
	UEngineSampler();
	~UEngineSampler();

	static std::shared_ptr<UEngineSampler> Create(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value);

	void BindToShaderSlot(EShaderType _Type, UINT _BindIndex);

	void UnbindFromShaderSlot(EShaderType _Type, UINT _BindIndex);

protected:
	void CreateSamplerState(const D3D11_SAMPLER_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> State = nullptr;

private:
	// delete Function
	UEngineSampler(const UEngineSampler& _Other) = delete;
	UEngineSampler(UEngineSampler&& _Other) noexcept = delete;
	UEngineSampler& operator=(const UEngineSampler& _Other) = delete;
	UEngineSampler& operator=(UEngineSampler&& _Other) noexcept = delete;
};

