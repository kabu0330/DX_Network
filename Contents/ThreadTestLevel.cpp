#include "PreCompile.h"
#include "ThreadTestLevel.h"

ThreadTestLevel::ThreadTestLevel()
{
}

ThreadTestLevel::~ThreadTestLevel()
{
}

void ThreadTestLevel::BeginPlay()
{
	Super::BeginPlay();
}

void ThreadTestLevel::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void ThreadTestLevel::LevelChangeStart()
{
	if (false == ThreadLoadingEnd)
	{
		Thread.Start("Loading", [this]()
			{
				for (size_t i = 0; i < 100000; i++)
				{
					UEngineDebug::OutPutString("Loading.....");
				}

				ThreadLoadingInit = true;
			});
	}
}
