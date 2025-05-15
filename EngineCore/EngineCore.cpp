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

	// ���������� �����Ͽ� ���α׷��� ����Ǳ� ���� ���� �޸𸮰� �����ǵ����Ͽ� static���� ���� �޸� ���� ������ �� ������� ��ü�Ѵ�.
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
			GEngine->Device.SetupEngineRenderingPipeline(); // �ʱ� ����

			// ������ ũ�� ����, ���ҽ� �ε�
			GEngine->Core->EngineStart(GEngine->Data);  
			GEngine->MainWindow.SetWindowPosAndScale(GEngine->Data.WindowPos, GEngine->Data.WindowSize);

			// ������ ����� ������ ���� ����ü���� �����ϹǷ� ������ �ھ ���ٿ� �Ŀ� ȣ���ϵ��� �и�
			GEngine->Device.InitializeRenderingPipelineOutput(GEngine->MainWindow); // ����ü�� �� ����� ����

			UEngineGUI::Init(); // IMGUI ����
		},
		[]()
		{
			// ���� ����
			EngineFrame();
		},
		[]()
		{
			// ���� ���� ��, �� �ѹ��� ȣ��
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
		MSGASSERT("Contents dll ������ �ε��� �� �����ϴ�.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = (INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("������ �ھ� ���ο� CreateContentsCoreDefine�� �������� �ʾҽ��ϴ�.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("������ �ھ� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineCore::EngineFrame()
{
	// ���� ������ �����Ұž�?
	if (true == GEngine->IsCurLevelReset)
	{
		GEngine->CurLevel = nullptr;
		GEngine->IsCurLevelReset = false;
	}

	if (nullptr != GEngine->NextLevel) // ����ü������ Level�� �����ϸ�
	{
		if (nullptr != GEngine->CurLevel) 
		{
			GEngine->CurLevel->LevelChangeEnd();
		}

		GEngine->CurLevel = GEngine->NextLevel; 

		GEngine->CurLevel->LevelChangeStart(); 
		GEngine->NextLevel = nullptr;
		GEngine->Timer.TimeStart(); // ��Ÿ Ÿ�ӵ� ó������ �ٽ� �����Ѵ�. Ȥ�� �� ������ �������
	}

	// 1. ��Ÿ Ÿ�� üũ
	GEngine->Timer.TimeCheck();
	float DeltaTime = GEngine->Timer.GetDeltaTime();

	// 2. �Է� üũ
	if (true == GEngine->MainWindow.IsFocus()) // �����찡 ��Ŀ���Ǿ��� ����
	{
		UEngineInput::KeyCheck(DeltaTime); // Ű �Է� üũ
	}
	else {
		UEngineInput::KeyReset();
	}

	// 3. ���� üũ
	UEngineSound::Update();

	// 4. ���� ��ȸ
	// Core���� Level�� �����ϴ� Actor, Renderer, Collision�� 'Windows�޽�������'���� �����ش�.
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

// ��� ��ȯ ������ ���� ���� �Լ��и�
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	if (true == GEngine->LevelMap.contains(_Name.data()))
	{
		MSGASSERT("���� �̸��� ������ �� ������� �����ϴ�." + std::string(_Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	GEngine->LevelMap.insert({ _Name.data(), Ptr}); // ������ ������ ��� LevelMap�� ����

	std::cout << "NewLevelCreate" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == GEngine->LevelMap.contains(UpperString))
	{
		MSGASSERT(std::string(_Name) + " �� �������� ���� �����Դϴ�. \n CreateLevel �Լ��� ����� ������ ���� �� OpenLevel �Լ��� ����ؾ� �մϴ�.");
		return;
	}

	GEngine->NextLevel = GEngine->LevelMap[UpperString];
}

// EngineCore.dll�� ������ �޸𸮿��� �ϹǷ� cpp ���Ͽ��� Getter �Լ��� �����Ѵ�. ������� �����ϴ� ����� �޸𸮸� �����ϴ���.
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
		DestroyLevel(_LevelName); // �����
		return false;
	}
	return true;
}

std::shared_ptr<ULevel> UEngineCore::ReadyToNextLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	std::map<std::string, std::shared_ptr<ULevel>>::iterator FindIter = GEngine->LevelMap.find(UpperName);
	GEngine->LevelMap.erase(FindIter); // ���� ������ Level ������������ ���ܽ�Ű��
	GEngine->IsCurLevelReset = true; // ���� �����ӱ��� ���� ������ ����д�.

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
