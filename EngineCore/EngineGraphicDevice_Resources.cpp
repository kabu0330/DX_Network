#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "EngineVertexBuffer.h"
#include "EngineIndexBuffer.h"
#include "Mesh.h"
#include "EngineBlend.h"
#include "EngineShader.h"
#include "EngineMaterial.h"
#include "EngineTexture.h"
#include "EngineFont.h"
#include "EngineDepthStencilState.h"

void UEngineGraphicDevice::SetupEngineRenderingPipeline()
{
	// 세팅 순서는 중요하지 않다.
	CreateDepthStencilStates();
	CreateSamplerStates();
	CreateDefaultShaders();
	CreateRasterizerStates();

	CreateBlendStates();

	CreateEngineMeshBuffers();
	CreateEngineMaterials();

	UEngineFont::LoadFont("궁서", "궁서"); // 기본 폰트
}

void UEngineGraphicDevice::CreateDepthStencilStates()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_LESS; // 깊이값이 더 작으면 통과
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		UEngineDepthStencilState::Create("BaseDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_LESS; // 깊이값이 더 작으면 통과
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("UIDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS; // 항상 통과
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("CollisionDebugDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS; // 항상 통과
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("TargetMerge", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("ALWAYS", Desc);
	}
}

void UEngineGraphicDevice::CreateSamplerStates()
{
		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 0~1사이만 유효
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // y
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // z // 3중 

		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);

	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("EngineShader"))
		{
			MSGASSERT("EngineShader 폴더를 찾지 못했습니다.");
			return;
		}

		// 텍스처 설정을 안해줬을 때 화면에 렌더링할 NS 텍스처 한 장 로드해준다.
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::LoadTexture(FilePath);
		}
	}
}

void UEngineGraphicDevice::CreateDefaultShaders()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");

	std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, {".fx", ".hlsl"});

	// 셰이더 파일을 분석하여 VS, PS 구분하여 데이터 저장
	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::AutoCompileShaderByNaming(ShaderFiles[i]);
	}
}

void UEngineGraphicDevice::CreateRasterizerStates()
{
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		UEngineRasterizerState::Create("EngineBase", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		UEngineRasterizerState::Create("CollisionDebugRas", Desc);
	}
}

void UEngineGraphicDevice::CreateBlendStates()
{
	D3D11_BLEND_DESC Desc = { 0 };

	Desc.AlphaToCoverageEnable = false;			// 멀티샘플링 알파 커버리지
	Desc.IndependentBlendEnable = true;			// 각 렌더 타겟별로 블렌딩 여부를 결정
	Desc.RenderTarget[0].BlendEnable = true;	// 블렌딩 활성화
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA 모두 출력
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 색상 추가 연산자 '+'
	// 알파블랜드의 기본 공식
	// SrcColor 1.0, 0.0f, 0.0f, 0.8f; * 0.8f 0.8f 0.8f 0.8f

	// SrcColor 0.0, 0.0f, 1.0f, 1.0f; * 1 - 0.8f,  1 - 0.8f, 1 - 0.8f, 1 - 0.8f

	// 알베도컬러 SrcColor 옵션 SrcFactor  BlendOp  DestColor  옵션 DestFactor  

	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			// 소스 알파값 사용
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		// 대상 알파값 반전 사용 (1 - 소스 알파값) = 대상 알파값

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			// 알파 연산
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// 알파 소스 값
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;			// 알파 대상 값

	UEngineBlend::Create("AlphaBlend", Desc);
}

void UEngineGraphicDevice::CreateEngineMeshBuffers()
{
	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("Rect", Vertexs);
	}

	{
		std::vector<unsigned int> Indices;

		Indices.push_back(0);
		Indices.push_back(1);
		Indices.push_back(2);

		Indices.push_back(1);
		Indices.push_back(3);
		Indices.push_back(2);
		UEngineIndexBuffer::Create("Rect", Indices);
	}

	// 포지션을 1로 하면 화면 전체를 가리는 메시를 만들 수 있다.
	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-1.0f, 1.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(1.0f, 1.0f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-1.0f, -1.0f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(1.0f, -1.0f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("FullRect", Vertexs);
	}

	{
		UMesh::Create("Rect");

		// FullRect 포스트 프로세싱용 화면 전체크기 만한 메시를 제작
		UMesh::CreateWithBuffers("FullRect", "FullRect", "Rect");
	}

	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(2);
		Vertexs[0] = FEngineVertex{ FVector(-0.5f, 0.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.0f, 0.0f), {1.0f , 0.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("Line", Vertexs);


		std::vector<unsigned int> Indices;

		Indices.push_back(0);
		Indices.push_back(1);
		UEngineIndexBuffer::Create("Line", Indices);
	}
}

void UEngineGraphicDevice::CreateEngineMaterials()
{
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");
		Mat->SetDepthStencilState("CollisionDebugDepth"); // 언제나 화면에 나오는 누구도 이녀석의 앞을 가릴수 없어.
		Mat->SetRasterizerState("CollisionDebugRas");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("TargetMerge");
		Mat->SetVertexShader("EngineTargetMergeShader.fx");
		Mat->SetPixelShader("EngineTargetMergeShader.fx");
		Mat->SetDepthStencilState("TargetMerge"); // 언제나 화면에 나오는 누구도 이녀석의 앞을 가릴수 없어.
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("WidgetMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetDepthStencilState("UIDepth");
	}
}