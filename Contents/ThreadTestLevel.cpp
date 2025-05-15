#include "PreCompile.h"
#include "ThreadTestLevel.h"
#include "TestGameMode.h"

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
}

void AThreadTestLevel::LevelChangeStart()
{
	if (false == ThreadLoadingEnd)
	{
		Thread.Start("Loading", [this]()
			{
				UEngineCore::GetThreadPool().WorkQueue([this]()
					{
						//UEngineTexture::LoadTexutre
					});
			});

	}
}
