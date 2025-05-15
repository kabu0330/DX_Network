#include "PreCompile.h"
#include "ThreadTestLevel.h"
#include "TestGameMode.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/HUD.h>

AThreadTestLevel::AThreadTestLevel()
{
}

AThreadTestLevel::~AThreadTestLevel()
{
}

void AThreadTestLevel::BeginPlay()
{
	Super::BeginPlay();
}

void AThreadTestLevel::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == ThreadLoadingInit)
	{
		if (0 == LoadingCount)
		{
			ThreadLoadingEnd = true;
		}

		if (true == ThreadLoadingEnd)
		{
			UEngineCore::OpenLevel("Title");
		}
	}
}

void AThreadTestLevel::LevelChangeStart()
{
	ThreadLoadingInit = false;
	if (false == ThreadLoadingEnd)
	{
		Thread.Start("Loading", [this]()
			{
				UEngineDirectory Dir;
				if (false == Dir.MoveParentToDirectory("ContentsResources"))
				{
					MSGASSERT("리소스 폴더를 찾지 못했습니다.");
					return;
				}
				Dir.Append("Image");
				std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

				LoadingCount = static_cast<int>(ImageFiles.size());

				for (size_t i = 0; i < ImageFiles.size(); i++)
				{
					std::string FilePath = ImageFiles[i].GetPathToString();

					UEngineCore::GetThreadPool().WorkQueue([this, FilePath]()
						{
							UEngineTexture::LoadTextureThreadSafe(FilePath);
							--(this->LoadingCount);
						});
				}

				for (size_t i = 0; i < 1000; i++)
				{
					UEngineDebug::OutPutString("Loading");
				}
				ThreadLoadingInit = true;
			});

	}
}
