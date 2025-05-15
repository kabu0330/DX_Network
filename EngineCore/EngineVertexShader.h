#pragma once
#include "EngineResources.h"
#include <Windows.h>
#include "EngineShader.h"

// Ό³Έν :
class UEngineVertexShader : public UEngineResources, public UEngineShader
{
public:
	UEngineVertexShader();
	~UEngineVertexShader();

	static std::shared_ptr<UEngineVertexShader> CreateVertexShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileName();

		return CreateVertexShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}

	ENGINEAPI static std::shared_ptr<UEngineVertexShader> CreateVertexShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	void VSSetShader();

protected:

private:
	ENGINEAPI void CreateVertexShader();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> ShaderRes = nullptr;

private:
	// delete Function
	UEngineVertexShader(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader(UEngineVertexShader&& _Other) noexcept = delete;
	UEngineVertexShader& operator=(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader& operator=(UEngineVertexShader&& _Other) noexcept = delete;
};

