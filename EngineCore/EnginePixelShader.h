#pragma once
#include "EngineResourceManager.h"
#include <Windows.h>
#include "EngineShader.h"

// ���� : ���̴� ���Ϸκ��� �ȼ� ���̴��� �������ϰ� ����Ʈ �ڵ�� ���÷��� ������ �����Ͽ�
//		  ������ ������������ PS ���Կ� ���ε��ϴ� Ŭ����
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

