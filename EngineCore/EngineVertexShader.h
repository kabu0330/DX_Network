#pragma once
#include "EngineResourceManager.h"
#include <Windows.h>
#include "EngineShader.h"

// ���� : ���̴� ���Ϸκ��� ���ؽ� ���̴��� �������ϰ� ����Ʈ �ڵ�� ���÷��� ������ �����Ͽ�
//		  ������ ������������ VS ���Կ� ���ε��ϴ� Ŭ����
class UEngineVertexShader : public UEngineResourceManager, public UEngineShader
{
public:
	UEngineVertexShader();
	~UEngineVertexShader();

	// ���ؽ� ���̴��� �����ϰ� DX�� ����ϴ� �Լ�
	static std::shared_ptr<UEngineVertexShader> CreateVertexShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileName();

		return CreateVertexShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}
	ENGINEAPI static std::shared_ptr<UEngineVertexShader> CreateVertexShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	// DX ������������ VS ���Կ� ������ ���ؽ� ���̴��� ���ε��ϴ� �Լ�
	void VSSetShader();

protected:

private:
	// ���� GPU ���� ���̴� ��ü�� �����ϴ� �Լ�
	ENGINEAPI void CreateVertexShader();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;

private:
	// delete Function
	UEngineVertexShader(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader(UEngineVertexShader&& _Other) noexcept = delete;
	UEngineVertexShader& operator=(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader& operator=(UEngineVertexShader&& _Other) noexcept = delete;
};

