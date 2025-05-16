#pragma once
#include "EngineResourceManager.h"
#include "EngineEnums.h"

#include "ThirdParty/DirectxTex/Inc/DirectXTex.h"

// 설명 : 텍스처와 뷰 객체
class UEngineTexture : public UEngineResourceManager
{
	friend class UEngineRenderTarget;

public:
	ENGINEAPI UEngineTexture();
	ENGINEAPI ~UEngineTexture();

	static std::shared_ptr<UEngineTexture> LoadTextureThreadSafe(std::string_view _Path)
	{
		UEnginePath EnginePath = UEnginePath(_Path);
		std::string FileName = EnginePath.GetFileName();
		return LoadTextureThreadSafe(FileName, _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineTexture> LoadTextureThreadSafe(std::string_view _Name, std::string_view _Path);

	static std::shared_ptr<UEngineTexture> LoadTexture(std::string_view _Path)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileName();

		return LoadTexture(FileName, _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineTexture> LoadTexture(std::string_view _Name, std::string_view _Path);

	ID3D11DepthStencilView* GetDSV()
	{
		return DSV.Get();
	}

	ID3D11RenderTargetView* GetRTV()
	{
		return RTV.Get();
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV.Get();
	}

	FVector GetTextureSize()
	{
		return Size;
	}

	void BindToShaderSlot(EShaderType _Type, UINT _BindIndex);
	void UnbindFromShaderSlot(EShaderType _Type, UINT _BindIndex);

	ENGINEAPI void CreateTextureWithView(const D3D11_TEXTURE2D_DESC& _Value);

	ENGINEAPI void CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D);

	ENGINEAPI void CreateRenderTargetView();
	ENGINEAPI void CreateShaderResourceView();
	ENGINEAPI void CreateDepthStencilView();
protected:

private:
	ENGINEAPI void LoadResource();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // GPU Texture Data

	FVector Size = FVector::ZERO;
	DirectX::TexMetadata Metadata = DirectX::TexMetadata();
	DirectX::ScratchImage ImageData = DirectX::ScratchImage();
	

	D3D11_TEXTURE2D_DESC Desc = {};
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV = nullptr; // 읽기
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV = nullptr; // 쓰기
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV = nullptr; // 깊이 테스트

private:
	// delete Function
	UEngineTexture(const UEngineTexture& _Other) = delete;
	UEngineTexture(UEngineTexture&& _Other) noexcept = delete;
	UEngineTexture& operator=(const UEngineTexture& _Other) = delete;
	UEngineTexture& operator=(UEngineTexture&& _Other) noexcept = delete;
};
