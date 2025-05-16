#pragma once
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineRasterizerState.h"
#include "EngineDepthStencilState.h"
#include "EngineBlend.h"

// ���� : ���ؽ� ���̴�, �ȼ� ���̴�, �����Ͷ�����, ���ĺ���, ���� ���ٽ�, ���������� ��� �ѵ� ��� ��Ƽ����
class UEngineMaterial : public UEngineResourceManager
{
public:
	UEngineMaterial();
	~UEngineMaterial();

	ENGINEAPI static std::shared_ptr<UEngineMaterial> Create(std::string_view _Name);

	ENGINEAPI std::shared_ptr<UEngineVertexShader> GetVertexShader()
	{
		return VertexShader;
	}
	ENGINEAPI void SetVertexShader(std::string_view _Name);

	ENGINEAPI std::shared_ptr<UEnginePixelShader> GetPixelShader()
	{
		return PixelShader;
	}
	ENGINEAPI void SetPixelShader(std::string_view _Name);

	ENGINEAPI std::shared_ptr<UEngineRasterizerState> GetRasterizerState()
	{
		return RasterizerState;
	}
	ENGINEAPI void SetRasterizerState(std::string_view _Name);

	ENGINEAPI std::shared_ptr<UEngineBlend> GetBlend()
	{
		return Blend;
	}
	ENGINEAPI void SetBlend(std::string_view _Name);

	ENGINEAPI std::shared_ptr<UEngineDepthStencilState> GetDepthStencilState()
	{
		return DepthState;
	}
	ENGINEAPI void SetDepthStencilState(std::string_view _Name);

	ENGINEAPI void IASetPrimitiveTopology();

protected:

private:
	std::shared_ptr<UEngineVertexShader> VertexShader;
	std::shared_ptr<UEnginePixelShader> PixelShader;
	std::shared_ptr<UEngineRasterizerState> RasterizerState;
	std::shared_ptr<UEngineBlend> Blend;
	std::shared_ptr<UEngineDepthStencilState> DepthState;

	D3D11_PRIMITIVE_TOPOLOGY TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
private:
	// delete Function
	UEngineMaterial(const UEngineMaterial& _Other) = delete;
	UEngineMaterial(UEngineMaterial&& _Other) noexcept = delete;
	UEngineMaterial& operator=(const UEngineMaterial& _Other) = delete;
	UEngineMaterial& operator=(UEngineMaterial&& _Other) noexcept = delete;

};

