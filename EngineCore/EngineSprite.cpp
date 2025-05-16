#include "PreCompile.h"
#include "EngineSprite.h"
#include "EngineBase/EngineDebug.h"
#include "EngineTexture.h"

UEngineSprite::UEngineSprite()
{
}

UEngineSprite::~UEngineSprite()
{
}

std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToFolder(std::string_view _Name, std::string_view _Path)
{
	UEngineDirectory Dir = _Path;

	std::vector<UEngineFile> Files = Dir.GetAllFile(false, { ".png"});

	if (0 == Files.size())
	{
		MSGASSERT("폴더 내 이미지가 존재하지 않습니다. \n이 함수는 폴더 내 연속된 스프라이트 이미지가 존재해야 합니다.");
		return nullptr;
	}

	std::shared_ptr<UEngineSprite> NewSpriteTexture = std::make_shared<UEngineSprite>();
	AddResource<UEngineSprite>(NewSpriteTexture, _Name, "");

	for (size_t i = 0; i < Files.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(Files[i].GetFileName());

		std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Find<UEngineTexture>(UpperName);

		if (nullptr == Texture)
		{
			MSGASSERT("텍스처 로드에 실패했습니다. " + UpperName);
			return nullptr;
		}

		NewSpriteTexture->SpriteTexture.push_back(Texture.get());

		FSpriteData SpriteData;
		SpriteData.CuttingPos = { 0.0f, 0.0f };
		SpriteData.CuttingSize = { 1.0f, 1.0f };
		SpriteData.Pivot = { 0.5f, 0.5f };
		NewSpriteTexture->SpriteDatas.push_back(SpriteData);
	}

	return NewSpriteTexture;
}

std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UEngineTexture> MetaFile = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == MetaFile)
	{
		std::string Name = _Name.data();
		std::string Ext = _DataFileExt.data();
		MSGASSERT(Name + Ext + " 의 Unity 메타 파일을 찾지 못했습니다. 파일 이름 또는 확장자 명을 확인해주세요.");
		return nullptr;
	}

	std::shared_ptr<UEngineSprite> NewSpriteTexture = std::make_shared<UEngineSprite>();
	AddResource<UEngineSprite>(NewSpriteTexture, _Name, "");

	UEnginePath ResourcePath = MetaFile->GetPath();
	std::string FileName = ResourcePath.GetFileName();
	FileName += _DataFileExt;
	ResourcePath.MoveParent();
	ResourcePath.Append(FileName);

	UEngineFile File = ResourcePath;

	File.FileOpen("rt");

	std::string Text = File.GetAllFileText();

	std::vector<std::string> SpriteDataTexts;

	size_t StartPosition = 0;
	while (true)
	{
		size_t RectIndex = Text.find("rect:", StartPosition);
		size_t AligIndex = Text.find("outline:", RectIndex);

		if (RectIndex == std::string::npos || AligIndex == std::string::npos)
		{
			break;
		}

		NewSpriteTexture->SpriteTexture.push_back(MetaFile.get());
		SpriteDataTexts.push_back(Text.substr(RectIndex, AligIndex - RectIndex));
		StartPosition = AligIndex;
	}

	FVector TexSize = MetaFile->GetTextureSize();

	std::vector<FSpriteData> TestData;

	for (size_t i = 0; i < SpriteDataTexts.size(); i++)
	{
		std::string Text = SpriteDataTexts[i];

		FSpriteData SpriteData;
		size_t Start = 0;

		{
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.CuttingPos.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
			SpriteData.CuttingPos.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "width:", "\n", Start);
			SpriteData.CuttingSize.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "height:", "\n", Start);
			SpriteData.CuttingSize.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "x:", ",", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "}", Start);
			SpriteData.Pivot.Y = static_cast<float>(atof(Number.c_str()));
		}
		// Y축 반전, 0이 맨 아래, 맨 위가 텍스처 최대 높이
		SpriteData.CuttingPos.Y = TexSize.Y - SpriteData.CuttingPos.Y - SpriteData.CuttingSize.Y;

		SpriteData.CuttingPos.X /= TexSize.X;
		SpriteData.CuttingPos.Y /= TexSize.Y;
		SpriteData.CuttingSize.X /= TexSize.X;
		SpriteData.CuttingSize.Y /= TexSize.Y;

		TestData.push_back(SpriteData);
	}

	NewSpriteTexture->SpriteDatas = TestData;

	return NewSpriteTexture;
}

UEngineTexture* UEngineSprite::GetTexture(size_t _Index /*= 0*/)
{
	return SpriteTexture[_Index];
}

ID3D11ShaderResourceView* UEngineSprite::GetSRV(size_t _Index/* = 0*/)
{
	return SpriteTexture[_Index]->GetSRV();
}

FVector UEngineSprite::GetSpriteScaleToReal(size_t _Index)
{
	if (SpriteDatas.size() <= _Index)
	{
		MSGASSERT("스프라이트의 인덱스를 초과하여 사용하려고 했습니다.");
		return FVector();
	}
	
	FVector Result;

	//                0~1사이의 비율이기 때문에
	Result.X = SpriteDatas[_Index].CuttingSize.X * SpriteTexture[_Index]->GetTextureSize().X;
	Result.Y = SpriteDatas[_Index].CuttingSize.Y * SpriteTexture[_Index]->GetTextureSize().Y;

	return Result;
}