#pragma once
#include <wrl.h>
#include <d3d11_4.h> 
#include <d3dcompiler.h> 
#include <EnginePlatform/EngineWindow.h>
#include <memory>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 



// ���� : GPU ����̽�, ���ؽ�Ʈ, ����� �� �⺻ GPU �ڿ� �ʱ�ȭ ������
class UEngineGraphicDevice
{
public:
	ENGINEAPI UEngineGraphicDevice();
	ENGINEAPI ~UEngineGraphicDevice();

	ENGINEAPI void Release();

	// GPU ����̽� �� ���ؽ�Ʈ ����
	void CreateDeviceAndContext();

	// VRAM ���� �ְ� ���� GPU �˻�
	IDXGIAdapter* FindHighPerformanceAdapter();

	// ������ ���������� �ʱ� ����
	ENGINEAPI void SetupEngineRenderingPipeline();
	
	// ����ü�� �� ����� ����
	ENGINEAPI void InitializeRenderingPipelineOutput(const UEngineWindow& _Window)
	{
		InitializeSwapChain(_Window);
		InitializeBackBufferRenderTargets();
	}

	// ����� �ʱ�ȭ �� ����Ÿ�� ���ε�
	void OMSetRenderTargetWithClear();

	// GPU ��� ���
	void Present();


	ENGINEAPI ID3D11Device* GetDevice()
	{
		return Device.Get();
	}
	ENGINEAPI ID3D11DeviceContext* GetContext()
	{
		return Context.Get();
	}
	ENGINEAPI std::shared_ptr<class UEngineRenderTarget> GetBackBufferTarget()
	{
		return BackBufferTarget;
	}

protected:

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter> MainAdapter = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;

	// ����� Ÿ�� = ����� �ؽ�ó + ���� �ؽ�ó + ���� Ÿ�� �� 
	std::shared_ptr<class UEngineRenderTarget> BackBufferTarget;

private:
	// ����ü�� �� ����� ����
	ENGINEAPI void InitializeSwapChain(const UEngineWindow& _Window);
	ENGINEAPI void InitializeBackBufferRenderTargets();

	// �������� �����ϴ� �⺻ ������ ���������� ����
	ENGINEAPI void CreateDefaultShaders(); // ���̴� ���� �� �ϳ��� �⺻�����߱� ������ Default�� �ٿ���.
	ENGINEAPI void CreateRasterizerStates();
	ENGINEAPI void CreateSamplerStates();
	
	ENGINEAPI void CreateDepthStencilStates();
	ENGINEAPI void CreateBlendStates();

	ENGINEAPI void CreateEngineMeshBuffers(); // DX���� �����ϴ� ������ �ƴϱ� ������ Engine�� �ٿ���.
	ENGINEAPI void CreateEngineMaterials();

	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;
};

