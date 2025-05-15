#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineSound.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentsCore.h"
#include "EngineResources.h"
#include "EngineConstantBuffer.h"
#include "EngineGUI.h"
#include "Level.h"
#include "GameInstance.h"

void UEngineCore::EngineStart(HINSTANCE _Instance, std::string_view _DllName)
{
	UEngineDebug::CheckMemoryLeak();

	// 지역변수로 생성하여 프로그램이 종료되기 전에 먼저 메모리가 정리되도록하여 static으로 인한 메모리 누수 문제를 이 방법으로 대체한다.
	UEngineCore EngineCore;
	GEngine = &EngineCore;

	GEngine->ThreadPool.Initialize();

	WindowInit(_Instance);
	LoadContentsDll(_DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			UEngineSound::Init();
			// UEngineDebug::StartConsole();
			
			GEngine->Device.CreateDeviceAndContext();	
			GEngine->Device.SetupEngineRenderingPipeline(); // 초기 설정

			// 윈도우 크기 세팅, 리소스 로드
			GEngine->Core->EngineStart(GEngine->Data);  
			GEngine->MainWindow.SetWindowPosAndScale(GEngine->Data.WindowPos, GEngine->Data.WindowSize);

			// 윈도우 사이즈가 결정된 이후 스왑체인을 생성하므로 컨텐츠 코어에 갔다온 후에 호출하도록 분리
			GEngine->Device.InitializeRenderingPipelineOutput(GEngine->MainWindow); // 스왑체인 및 백버퍼 생성

			UEngineGUI::Init(); // IMGUI 생성
		},
		[]()
		{
			// 게임 실행
			EngineFrame();
		},
		[]()
		{
			// 게임 종료 시, 딱 한번만 호출
			EngineEnd();
		});	
}

void UEngineCore::WindowInit(HINSTANCE _Instance)
{
	UEngineWindow::EngineWindowInit(_Instance);
	GEngine->MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContentsDll(std::string_view _DllName)
{
	UEngineDirectory Dir;
	Dir.MoveParentToDirectory("Build");
	Dir.Move("bin/x64");

#ifdef _DEBUG
	Dir.Move("Debug");
#else
	Dir.Move("Release");
#endif

	UEngineFile File = Dir.GetFile(_DllName);
	std::string FullPath = File.GetPathToString();

	GEngine->ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == GEngine->ContentsDLL)
	{
		MSGASSERT("Contents dll 파일을 로드할 수 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = (INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine을 정의하지 않았습니다.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::EngineFrame()
{
	// 현재 레벨을 리셋할거야?
	if (true == GEngine->IsCurLevelReset)
	{
		GEngine->CurLevel = nullptr;
		GEngine->IsCurLevelReset = false;
	}

	if (nullptr != GEngine->NextLevel) // 레벨체인지할 Level이 존재하면
	{
		if (nullptr != GEngine->CurLevel) 
		{
			GEngine->CurLevel->LevelChangeEnd();
		}

		GEngine->CurLevel = GEngine->NextLevel; 

		GEngine->CurLevel->LevelChangeStart(); 
		GEngine->NextLevel = nullptr;
		GEngine->Timer.TimeStart(); // 델타 타임도 처음부터 다시 갱신한다. 혹시 모를 오류가 있을까봐
	}

	// 1. 델타 타임 체크
	GEngine->Timer.TimeCheck();
	float DeltaTime = GEngine->Timer.GetDeltaTime();

	// 2. 입력 체크
	if (true == GEngine->MainWindow.IsFocus()) // 윈도우가 포커스되었을 때만
	{
		UEngineInput::KeyCheck(DeltaTime); // 키 입력 체크
	}
	else {
		UEngineInput::KeyReset();
	}

	// 3. 사운드 체크
	UEngineSound::Update();

	// 4. 레벨 순회
	// Core에서 Level이 관리하는 Actor, Renderer, Collision를 'Windows메시지루프'에서 돌려준다.
	GEngine->CurLevel->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);

	GEngine->CurLevel->Collision(DeltaTime);
	GEngine->CurLevel->Release(DeltaTime);
}

void UEngineCore::EngineEnd()
{
	UEngineGUI::Release();

	GEngine->Device.Release();

	UEngineResources::Release();
	UEngineConstantBuffer::Release();
	UEngineSound::Release();

	GEngine->CurLevel = nullptr;
	GEngine->NextLevel = nullptr;
	GEngine->LevelMap.clear();

	UEngineDebug::EndConsole();
}

// 헤더 순환 참조를 막기 위한 함수분리
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	if (true == GEngine->LevelMap.contains(_Name.data()))
	{
		MSGASSERT("같은 이름의 레벨을 또 만들수는 없습니다." + std::string(_Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	GEngine->LevelMap.insert({ _Name.data(), Ptr}); // 생성된 레벨은 모두 LevelMap에 저장

	std::cout << "NewLevelCreate" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == GEngine->LevelMap.contains(UpperString))
	{
		MSGASSERT(std::string(_Name) + " 은 생성되지 않은 레벨입니다. \n CreateLevel 함수를 사용해 레벨을 생성 후 OpenLevel 함수를 사용해야 합니다.");
		return;
	}

	GEngine->NextLevel = GEngine->LevelMap[UpperString];
}

// EngineCore.dll에 생성된 메모리여야 하므로 cpp 파일에서 Getter 함수를 구현한다. 헤더에서 구현하니 복사된 메모리를 참조하더라.
UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return GEngine->Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return GEngine->MainWindow;
}

float UEngineCore::GetDeltaTime()
{
	float Time = GEngine->Timer.GetDeltaTime();

	return Time;
}

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::GetAllLevelMap()
{
	return GEngine->LevelMap;
}

UGameInstance* UEngineCore::GetGameInstance()
{
	return GEngine->GameInstance.get();
}

UEngineWorkThreadPool& UEngineCore::GetThreadPool()
{
	return GEngine->ThreadPool;
}

UEngineCore* GEngine = nullptr;

FVector UEngineCore::GetScreenScale()
{
	return GEngine->Data.WindowSize;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::SetGameInstance(std::shared_ptr<UGameInstance> _Inst)
{
	GEngine->GameInstance = _Inst;
}

bool UEngineCore::IsCurLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	if (GEngine->CurLevel->GetName() != UpperName) 
	{
		DestroyLevel(_LevelName); // 지우고
		return false;
	}
	return true;
}

std::shared_ptr<ULevel> UEngineCore::ReadyToNextLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	std::map<std::string, std::shared_ptr<ULevel>>::iterator FindIter = GEngine->LevelMap.find(UpperName);
	GEngine->LevelMap.erase(FindIter); // 현재 레벨을 Level 관리구조에서 제외시키고
	GEngine->IsCurLevelReset = true; // 다음 프레임까지 현재 레벨을 살려둔다.

	return 	GEngine->NextLevel;
}

void UEngineCore::SetNextLevel(std::shared_ptr<class ULevel> _NextLevel)
{
	GEngine->NextLevel = _NextLevel;
}

void UEngineCore::DestroyLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	if (false == GEngine->LevelMap.contains(UpperName))
	{
		return;
	}

	std::map<std::string, std::shared_ptr<class ULevel>>::iterator FindIter = GEngine->LevelMap.find(UpperName);

	if (nullptr != FindIter->second)
	{
		FindIter->second = nullptr;
	}

	GEngine->LevelMap.erase(FindIter);
}
