#pragma once
#include "EngineResourceManager.h"
#include <Windows.h>
#include "EngineShader.h"

// 설명 : 셰이더 파일로부터 픽셀 셰이더를 컴파일하고 바이트 코드와 리플렉션 정보를 포함하여
//		  렌더링 파이프라인의 PS 슬롯에 바인딩하는 클래스
class UEnginePixelShader : public UEngineResourceManager, public UEngineShader
{
public:
	UEnginePixelShader();
	~UEnginePixelShader();

	static std::shared_ptr<UEnginePixelShader> CreatePixelShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileName();

		return CreatePixelShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}

	ENGINEAPI static std::shared_ptr<UEnginePixelShader> CreatePixelShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	void PSSetShader();

protected:

private:
	ENGINEAPI void CreatePixelShader();

	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;

private:
	// delete Function
	UEnginePixelShader(const UEnginePixelShader& _Other) = delete;
	UEnginePixelShader(UEnginePixelShader&& _Other) noexcept = delete;
	UEnginePixelShader& operator=(const UEnginePixelShader& _Other) = delete;
	UEnginePixelShader& operator=(UEnginePixelShader&& _Other) noexcept = delete;
};

