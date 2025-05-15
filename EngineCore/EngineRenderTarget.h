#pragma once
#include "EngineResources.h"
#include "EngineTexture.h"
#include "RenderUnit.h"
#include "EngineGraphicDevice.h"

// 설명 : 렌더 타겟 뷰 생성, 깊이버퍼 생성, 출력 병합 후 렌더링 파이프라인으로 전송
class UEngineRenderTarget : public UEngineResources
{
public:
	UEngineRenderTarget();
	~UEngineRenderTarget();

	ENGINEAPI void CreateRenderTargetView(float4 _Scale, float4 _ClearColor = float4::NONE, DXGI_FORMAT _Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	ENGINEAPI void CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D);

	ENGINEAPI void CreateDepthTexture(int _Index = 0);

	ENGINEAPI void ClearRenderTargetView();

	ENGINEAPI void OMSetRenderTargets();

	ENGINEAPI void SetClearColor(float4 _ClearColor)
	{
		ClearColor = _ClearColor;
	}

	// 특정 렌더 타겟에 있는 데이터를 복사한다. 결국 최종 렌더 타겟에 그려져야 하니까.
	ENGINEAPI void CopyTo(std::shared_ptr<UEngineRenderTarget> _Target);

	ENGINEAPI void MergeTo(std::shared_ptr<UEngineRenderTarget> _Target);

protected:

private:
	float4 ClearColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<std::shared_ptr<class UEngineTexture>> AllRenderTargetTextures;
	std::vector<ID3D11RenderTargetView*> AllRTVs;
	std::vector<ID3D11ShaderResourceView*> AllSRVs;

	std::shared_ptr<class UEngineTexture> DepthTexture;

	URenderUnit TargetUnit;

private:
	// delete Function
	UEngineRenderTarget(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget(UEngineRenderTarget&& _Other) noexcept = delete;
	UEngineRenderTarget& operator=(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget& operator=(UEngineRenderTarget&& _Other) noexcept = delete;
};

