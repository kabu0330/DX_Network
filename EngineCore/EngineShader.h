#pragma once
#include "EngineEnums.h"
#include <EngineBase/EngineFile.h>
#include "ShaderBindingManager.h"

// ���� : ���̴� ������ �ڵ� �������ϰ�, ��� ���� / �ؽ�ó / ���÷��� ���̴� ���÷��� ������� �ڵ� ����
//		  ���ε� ������ �����ϴ� ���̴� ���� Ŭ����
class UEngineShader
{
	friend class UEngineVertexShader;

public:
	UEngineShader();
	~UEngineShader();

	// ���̴� ���� ���� _VS, _PS ���̹� ��Ģ���� ���ؽ� ���̴��� �ȼ� ���̴��� �ڵ� �и� ������
	ENGINEAPI static void AutoCompileShaderByNaming(UEngineFile& _File);

	UShaderBindingManager ShaderBindingManager;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	};

protected:
	EShaderType ShaderType = EShaderType::MAX_SHADER_TYPE;
	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // �߰� ������ �����
	std::string EntryName;

	// ���ε� ����(�̸�, �ε���, Ÿ��)�� ����ϴ� �Լ�
	void ReflectAndBindShaderResources();

private:
	// delete Function
	UEngineShader(const UEngineShader& _Other) = delete;
	UEngineShader(UEngineShader&& _Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& _Other) = delete;
	UEngineShader& operator=(UEngineShader&& _Other) noexcept = delete;
};

