#include "PreCompile.h"
#include "ContentResource.h"
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/EngineFont.h>
#include <EngineCore/EngineTexture.h>

void UContentResource::LoadResource()
{
	{	// 1. �̹��� ���� �ε�
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
	//	// �� ���ҽ�
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
	//	// �� ���ҽ�
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

	//{	// ���� �ε�
	//	UEngineDirectory Dir;
	//	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	//	{
	//		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
		// ��Ʈ
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/TrajanPro-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("TrajanPro-Regular", FilePath);
	}
	{
		// ��Ʈ
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/NotoSerifCJKsc-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("NotoSerifCJKsc-Regular", FilePath);
	}
	{
		// ��Ʈ
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/Perpetua.ttf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("Perpetua", FilePath);
	}
	{
		// ��Ʈ
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
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

void UContentResource::LoadResourceDirectory()
{
	LoadResource(); // ���� 1ȸ ���ҽ� ������ �ε��ؾ� �Ѵ�.
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