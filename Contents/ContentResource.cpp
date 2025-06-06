#include "PreCompile.h"
#include "ContentResource.h"
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/EngineFont.h>
#include <EngineCore/EngineTexture.h>

void UContentResource::LoadResource()
{
	{	// 1. 이미지 파일 로드
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}

		Dir.Append("Image");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::LoadTexture(FilePath);
		}
	}

	//{
	//	// 맵 리소스
	//	UEngineDirectory Dir;
	//	Dir.MoveParentToDirectory("ContentsResources");
	//	Dir.Append("MapObjectResources");

	//	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
	//	for (size_t i = 0; i < ImageFiles.size(); i++)
	//	{
	//		std::string FilePath = ImageFiles[i].GetPathToString();
	//		UEngineTexture::LoadTexture(FilePath);
	//	}
	//}

	//{
	//	// 맵 리소스
	//	UEngineDirectory Dir;
	//	Dir.MoveParentToDirectory("ContentsResources");
	//	Dir.Append("MapData");

	//	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
	//	for (size_t i = 0; i < ImageFiles.size(); i++)
	//	{
	//		std::string FilePath = ImageFiles[i].GetPathToString();
	//		UEngineTexture::LoadTexture(FilePath);
	//	}
	//}	

	//{	// 사운드 로드
	//	UEngineDirectory Dir;
	//	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	//	{
	//		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
	//		return;
	//	}
	//	Dir.Append("Sound");


	//	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".wav", ".mp3" });

	//	for (size_t i = 0; i < ImageFiles.size(); i++)
	//	{
	//		std::string FilePath = ImageFiles[i].GetPathToString();
	//		UEngineSound::LoadSound(FilePath);
	//	}
	//}
}

void UContentResource::LoadFont()
{
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/TrajanPro-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("TrajanPro-Regular", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/NotoSerifCJKsc-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("NotoSerifCJKsc-Regular", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/Perpetua.ttf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("Perpetua", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/TrajanPro-Bold.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("TrajanPro-Bold", FilePath);
	}
}

void UContentResource::LoadContentsResource(std::string_view _Path)
{
	std::string Path = _Path.data();
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

void UContentResource::LoadResourceDirectory()
{
	LoadResource(); // 최초 1회 리소스 폴더를 로드해야 한다.
	LoadFont();

	//LoadContentsResource("Image/Knight/Idle");

}

void UContentResource::LoadFolder()
{
	//UEngineDirectory TitleMain;
	//TitleMain.MoveParentToDirectory("ContentsResources//Image//Title");
	//TitleMain.Append("TitleBackGround");
}

void UContentResource::LoadSprite()
{
	UEngineSprite::CreateSpriteToMeta("minos00.png", ".smeta");
}

void UContentResource::LoadShaderResource()
{
	// Shader
	{
		UEngineDirectory CurDir;
		CurDir.MoveParentToDirectory("ContentsShader");

		std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

		for (size_t i = 0; i < ShaderFiles.size(); i++)
		{
			UEngineShader::AutoCompileShaderByNaming(ShaderFiles[i]);
		}
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
		Mat->SetVertexShader("ContentsShader.fx");
		Mat->SetPixelShader("ContentsShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyCollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");

		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}
}

UContentResource::UContentResource()
{
}

UContentResource::~UContentResource()
{
}