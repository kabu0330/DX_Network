#pragma once
#include "EngineTexture.h"
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineDirectory.h>
#include "EngineResourceManager.h"
#include "EngineDataStruct.h"
#include "EngineDataStruct.h"

// 설명 :
class UEngineSprite : public UEngineResourceManager
{
public:
	UEngineSprite();
	~UEngineSprite();

	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToFolder(std::string_view _Path)
	{
		UEngineDirectory Dir = _Path;

		return CreateSpriteToFolder(Dir.GetDirectoryName(), _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToFolder(std::string_view _Name, std::string_view _Path);

	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt);

	ENGINEAPI UEngineTexture* GetTexture(size_t _Index = 0);

	ENGINEAPI ID3D11ShaderResourceView* GetSRV(size_t _Index = 0);

	ENGINEAPI FSpriteData GetSpriteData(size_t _Index)
	{
		if (SpriteDatas.size() <= _Index)
		{
			std::string Size = std::to_string(SpriteDatas.size());
			std::string Index = std::to_string(_Index);			
			MSGASSERT("스프라이트 인덱스를 초과한 접근입니다. \n Max : " + Size + ", 요청 : " + Index);
			return FSpriteData();
		}

		return SpriteDatas[_Index];
	}

	ENGINEAPI FVector GetSpriteScaleToReal(size_t _Index);

	ENGINEAPI size_t GetSpriteCount()
	{
		return SpriteTexture.size();
	}

protected:

private:
	std::vector<UEngineTexture*> SpriteTexture;
	std::vector<FSpriteData> SpriteDatas;

private:
	// delete Function
	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;
};

