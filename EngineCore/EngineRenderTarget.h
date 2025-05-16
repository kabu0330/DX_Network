#pragma once
#include "EngineResourceManager.h"
#include "EngineTexture.h"
#include "RenderUnit.h"
#include "EngineGraphicDevice.h"

class UPostEffect
{
	friend class UEngineRenderTarget;
public:
	URenderUnit RenderUnit;
	UEngineRenderTarget* ResultTarget = nullptr;
	bool IsActive = true;

protected:
	ENGINEAPI virtual void Init() = 0
	{

	}
	ENGINEAPI virtual void Effect(class UEngineCamera* _Camera, float _DeltaTime) = 0
	{

	}
};



// ���� : ���� Ÿ�� �� ����, ���̹��� ����, ��� ���� �� ������ �������������� ����
class UEngineRenderTarget : public UEngineResourceManager
{
public:
	ENGINEAPI UEngineRenderTarget();
	ENGINEAPI ~UEngineRenderTarget();

	ENGINEAPI void CreateRenderTargetView(float4 _Scale, float4 _ClearColor = float4::NONE, DXGI_FORMAT _Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	ENGINEAPI void CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D);

	ENGINEAPI void CreateDepthTexture(int _Index = 0);

	ENGINEAPI void ClearRenderTargets();

	ENGINEAPI void OMSetRenderTargets();

	ENGINEAPI void SetClearColor(float4 _ClearColor)
	{
		ClearColor = _ClearColor;
	}

	// Ư�� ���� Ÿ�ٿ� �ִ� �����͸� �����Ѵ�. �ᱹ ���� ���� Ÿ�ٿ� �׷����� �ϴϱ�.
	ENGINEAPI void CopyTo(std::shared_ptr<UEngineRenderTarget> _Target);
	ENGINEAPI void MergeTo(std::shared_ptr<UEngineRenderTarget> _Target);

	UEngineTexture* GetTexture(int _Index = 0)
	{
		return AllRenderTargetTextures[_Index].get();
	}

protected:

private:
	float4 ClearColor = float4(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<std::shared_ptr<class UEngineTexture>> AllRenderTargetTextures;
	std::vector<ID3D11RenderTargetView*> AllRTVs;
	std::vector<ID3D11ShaderResourceView*> AllSRVs;

	std::shared_ptr<class UEngineTexture> DepthTexture;

	URenderUnit TargetUnit;


	// ����Ʈ ����Ʈ
public:
	template<typename EffectType>
	void AddPostEffet()
	{
		std::shared_ptr<EffectType> NewEffect = std::make_shared<EffectType>();
		std::shared_ptr<UPostEffect> PostEffect = std::dynamic_pointer_cast<UPostEffect>(NewEffect);

		PostEffect->ResultTarget = this;
		PostEffect->Init();
		PostEffects.push_back(NewEffect);
	}

	void ShowPostEffect(class UEngineCamera* _Camera, float _DeltaTime);

	std::shared_ptr<UPostEffect> GetPostEffect(int _Index)
	{
		return PostEffects[_Index];
	}

private:
	std::vector<std::shared_ptr<UPostEffect>> PostEffects;


private:
	// delete Function
	UEngineRenderTarget(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget(UEngineRenderTarget&& _Other) noexcept = delete;
	UEngineRenderTarget& operator=(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget& operator=(UEngineRenderTarget&& _Other) noexcept = delete;
};

