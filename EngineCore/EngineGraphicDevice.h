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



// 설명 : GPU 디바이스, 컨텍스트, 백버퍼 및 기본 GPU 자원 초기화 관리자
class UEngineGraphicDevice
{
public:
	ENGINEAPI UEngineGraphicDevice();
	ENGINEAPI ~UEngineGraphicDevice();

	ENGINEAPI void Release();

	// GPU 디바이스 및 컨텍스트 생성
	void CreateDeviceAndContext();

	// VRAM 기준 최고 성능 GPU 검색
	IDXGIAdapter* FindHighPerformanceAdapter();

	// 렌더링 파이프라인 초기 설정
	ENGINEAPI void SetupEngineRenderingPipeline();
	
	// 스왑체인 및 백버퍼 구성
	ENGINEAPI void InitializeRenderingPipelineOutput(const UEngineWindow& _Window)
	{
		InitializeSwapChain(_Window);
		InitializeBackBufferRenderTargets();
	}

	// 백버퍼 초기화 및 렌더타겟 바인딩
	void OMSetRenderTargetWithClear();

	// GPU 결과 출력
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

	// 백버퍼 타겟 = 백버퍼 텍스처 + 깊이 텍스처 + 렌더 타겟 뷰 
	std::shared_ptr<class UEngineRenderTarget> BackBufferTarget;

private:
	// 스왑체인 및 백버퍼 생성
	ENGINEAPI void InitializeSwapChain(const UEngineWindow& _Window);
	ENGINEAPI void InitializeBackBufferRenderTargets();

	// 엔진에서 제공하는 기본 렌더링 파이프라인 설정
	ENGINEAPI void CreateDefaultShaders(); // 셰이더 파일 딱 하나만 기본설정했기 때문에 Default를 붙였다.
	ENGINEAPI void CreateRasterizerStates();
	ENGINEAPI void CreateSamplerStates();
	
	ENGINEAPI void CreateDepthStencilStates();
	ENGINEAPI void CreateBlendStates();

	ENGINEAPI void CreateEngineMeshBuffers(); // DX에서 제공하는 개념이 아니기 때문에 Engine을 붙였다.
	ENGINEAPI void CreateEngineMaterials();

	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;
};

