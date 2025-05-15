#pragma once
#include "EngineEnums.h"
#include <EngineBase/EngineFile.h>
#include "ShaderBindingManager.h"

// 설명 :
class UEngineShader
{
	friend class UEngineVertexShader;

public:
	UEngineShader();
	~UEngineShader();

	ENGINEAPI static void AutoCompileShaderByNaming(UEngineFile& _File);

	UShaderBindingManager ShaderResources;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	};

protected:
	EShaderType ShaderType = EShaderType::MAX_SHADER_TYPE;
	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // 중간 컴파일 결과물
	std::string EntryName;

	void ShaderResCheck();

private:
	// delete Function
	UEngineShader(const UEngineShader& _Other) = delete;
	UEngineShader(UEngineShader&& _Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& _Other) = delete;
	UEngineShader& operator=(UEngineShader&& _Other) noexcept = delete;
};

