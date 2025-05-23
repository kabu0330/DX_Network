#include "PreCompile.h"
#include "Level.h"
#include "Actor.h"
#include "Renderer.h"
#include "Collision.h"
#include "EngineCore.h"
#include "EngineCamera.h"
#include "CameraActor.h"
#include "EngineGUI.h"
#include "EngineFont.h"
#include "EngineRenderTarget.h"
#include "HUD.h"

ULevel::ULevel()
{
	SpawnCamera(EEngineCameraType::MAIN_CAMERA); // ����ī�޶�

	std::shared_ptr<ACameraActor> UI_CAMERA = SpawnCamera(EEngineCameraType::UI_CAMERA); // UI ī�޶�
	UI_CAMERA->GetCameraComponent()->SetProjectionType(EProjectionType::ORTHOGRAPHIC); // UIī�޶�� �������� ���Ѵ�.

	// ȭ�鿡 �׷��� ���� ����Ÿ���� �����.
	LastRenderTarget = std::make_shared<UEngineRenderTarget>();
	LastRenderTarget->CreateRenderTargetView(UEngineCore::GetScreenScale());
	LastRenderTarget->CreateDepthTexture();
}

void ULevel::Tick(float _DeltaTime)
{
	{
		std::lock_guard PacketLockGuard(PacketLock);
		while (0 != PacketQueue.size())
		{
			std::shared_ptr<UEngineProtocol> Packet = PacketQueue.front();
			GetGameMode()->GetNetwork()->GetDispatcher().ProcessPacket(Packet);
			PacketQueue.pop();
		}
	}

	if (GetMainCamera()->IsFreeCamera()) // ���� ī�޶� �Ѹ� ������ Tick�� �Ͻ�����
	{
		return;
	}

	// Tick�� ������ ����, �ش� �����ӿ� ������ Actor�� BeginPlay�� ���� ȣ���Ѵ�.
	std::list<std::shared_ptr<class AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<class AActor>>::iterator EndIter = BeginPlayList.end();
	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive()) // ��Ȱ��ȭ�� ���ʹ� �ϴ� ����
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter); 
		CurActor->BeginPlay(); // ȣ��

		if (nullptr != CurActor->Parent)
		{
			continue;
		}

		AllActorList.push_back(CurActor); // BeginPlay�� ȣ��� Actor�� Tick�� ���� ActorList�� ����.
	}

	// BeginPlay�� ȣ��� Actor
	for (std::shared_ptr<AActor> CurActor : AllActorList)
	{
		if (false == CurActor->IsActive()) // ��Ȱ��ȭ�� Actor�� Tick�� ������ �ʴ´�.
		{
			continue;
		}

		CurActor->Tick(_DeltaTime);
	}
}

void ULevel::Render(float _DeltaTime)
{
	UEngineCore::GetDevice().OMSetRenderTargetWithClear(); // ����� �ʱ�ȭ �� OM�ܰ迡�� ����� RTV�� DSV ����

	LastRenderTarget->ClearRenderTargets(); // ���� ��� ȭ�鵵 ȭ�� �� �� ����

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		if (Camera.first == static_cast<int>(EEngineCameraType::UI_CAMERA)) 	// UIī�޶�� ���� ������ �����ش�.
		{
			continue;
		}

		if (false == Camera.second->IsActive())
		{
			continue;
		}

		Camera.second->Tick(_DeltaTime); // View ��İ� Projection ��� ���
		Camera.second->GetCameraComponent()->Render(_DeltaTime);

		// Ư�� ī�޶� ����Ʈ ����Ʈ
		 //Camera.second->ShowPostEffect();

		Camera.second->GetCameraComponent()->CameraTarget->MergeTo(LastRenderTarget); // ������ ��������������
	}

	// ���⼭ �ϸ� ȭ�� ��ü ����Ʈ ����Ʈ ����
	//LastRenderTarget->ShowPostEffect(); 

	if (true == Cameras.contains(static_cast<int>(EEngineCameraType::UI_CAMERA))) // UIī�޶�� ���� ������ �����ش�.
	{
		std::shared_ptr<ACameraActor> CameraActor = Cameras[static_cast<int>(EEngineCameraType::UI_CAMERA)];
		if (true == CameraActor->IsActive()) // UIī�޶� ��Ƽ�� �����϶��� ������.
		{
			std::shared_ptr<UEngineCamera> CameraComponent = Cameras[static_cast<int>(EEngineCameraType::UI_CAMERA)]->GetCameraComponent();

			CameraActor->Tick(_DeltaTime); 

			CameraComponent->CameraTarget->ClearRenderTargets(); // ȭ�鵵 �����
			CameraComponent->CameraTarget->OMSetRenderTargets(); // ����Ÿ�� �����ϰ�

			HUD->UIRender(CameraComponent.get(), _DeltaTime); // ������ ƽ, ���� ������

			CameraComponent->CameraTarget->MergeTo(LastRenderTarget); // ���� ��º����ϰ�
		}
	}
	else
	{
		MSGASSERT("UIī�޶� �������� �ʽ��ϴ�. ���� �����Դϴ�. UIī�޶� �������ּ���.");
		return;
	}

	// ����� ���� Ÿ�� ��� ����
	std::shared_ptr<UEngineRenderTarget> BackBuffer = UEngineCore::GetDevice().GetBackBufferTarget();
	LastRenderTarget->MergeTo(BackBuffer);

	// ����� ������ : �ַ� �ݸ���
	{
		std::shared_ptr<class ACameraActor> Camera = GetMainCamera();

		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			for (std::shared_ptr<UCollision>& _Collision : List)
			{
				if (false == _Collision->IsActive())
				{
					continue;
				}

				_Collision->DebugRender(Camera->GetCameraComponent().get(), _DeltaTime); // �����
			}
		}
	}

	// IMGUI ������
	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this, _DeltaTime);
	}

	UEngineCore::GetDevice().Present(); // ����ü���� �����ϴ� ����ۿ� ����Ʈ���۸� ��ȯ(Swap) 
	// ����Ʈ����(������ â)�� ���
}

void ULevel::Collision(float _DeltaTime)
{
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinks)
	{
		const std::string& LeftProfile = Links.first;

		std::list<std::string>& LinkSecond = Links.second;

		for (std::string& RightProfile : LinkSecond)
		{
			std::list<std::shared_ptr<class UCollision>>& LeftList = CheckCollisions[LeftProfile];
			std::list<std::shared_ptr<class UCollision>>& RightList = Collisions[RightProfile];

			for (std::shared_ptr<class UCollision>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<class UCollision>& RightCollision : RightList)
				{
					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					LeftCollision->CollisionEventCheck(RightCollision);
				}
			}
		}
	}
}

void ULevel::Release(float _DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		Camera.second->GetCameraComponent()->Release(_DeltaTime);
	}

	{
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			std::list<std::shared_ptr<UCollision>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UCollision>>::iterator EndIter = List.end();

			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				// ������Ʈ�� �޸𸮸� ������ �� �ִ� ������ ������ ���͸� ������ �ִ�.
				StartIter = List.erase(StartIter);
			}
		}
	}
	{
		std::list<std::shared_ptr<AActor>>& List = AllActorList;

		std::list<std::shared_ptr<AActor>>::iterator StartIter = List.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = List.end();

		for (; StartIter != EndIter; )
		{
			if (nullptr != (*StartIter)->Parent)
			{
				StartIter = List.erase(StartIter);
				continue;
			}

			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			// ���� delete�� �ʿ� ����.
			StartIter = List.erase(StartIter);
		}
	}
}

void ULevel::ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	if (false == Cameras.contains(_CameraOrder))
	{
		MSGASSERT("�������� �ʴ� ī�޶� �������� ����������� �߽��ϴ�.");
		return;
	}

	std::shared_ptr<ACameraActor> Camera = Cameras[_CameraOrder];

	Camera->GetCameraComponent()->ChangeRenderGroup(_PrevGroupOrder, _Renderer);
}

std::shared_ptr<class ACameraActor> ULevel::SpawnCamera(int _Order)
{
	std::shared_ptr<ACameraActor> Camera = std::make_shared<ACameraActor>();

	if (true == Cameras.contains(_Order))
	{
		MSGASSERT("�̹� �����ϴ� ī�޶� Order�Դϴ�. �ٸ� ���� �Է����ּ���. : " + std::to_string(_Order));
		return nullptr;
	}

	Camera->BeginPlay();

	Cameras.insert({ _Order , Camera });
	return Camera;
}

void ULevel::LevelChangeStart()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeStart();
	}
	for (std::shared_ptr<class AActor> Actor : AllActorList)
	{
		Actor->LevelChangeStart();
	}
}

void ULevel::LevelChangeEnd()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeEnd();
	}
	for (std::shared_ptr<class AActor> Actor : AllActorList)
	{
		Actor->LevelChangeEnd();
	}
}

void ULevel::CreateCollisionProfile(std::string_view _ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	Collisions[UpperName];
}

void ULevel::LinkCollisionProfile(std::string_view _LeftProfileName, std::string_view _RightProfileName)
{
	std::string LeftUpperName = UEngineString::ToUpper(_LeftProfileName);
	std::string RightUpperName = UEngineString::ToUpper(_RightProfileName);

	CollisionLinks[LeftUpperName].push_back(RightUpperName);
}

void ULevel::PushCollisionProfileEvent(std::shared_ptr<class URenderer> _Renderer)
{

}

void ULevel::ChangeCollisionProfileName(std::string_view _ProfileName, std::string_view _PrevProfileName, std::shared_ptr<UCollision> _Collision)
{
	if (false == Collisions.contains(_ProfileName.data()))
	{
		MSGASSERT("�������� �ʴ� �ݸ��� �׷쿡 �������� ����������� �߽��ϴ�.");
		return;
	}

	std::string PrevUpperName = UEngineString::ToUpper(_PrevProfileName);

	if (_PrevProfileName != "")
	{
		std::list<std::shared_ptr<UCollision>>& PrevCollisionGroup = Collisions[PrevUpperName];
		PrevCollisionGroup.remove(_Collision);
	}

	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	std::list<std::shared_ptr<UCollision>>& CollisionGroup = Collisions[UpperName];
	CollisionGroup.push_back(_Collision);
}

void ULevel::InitLevel(AGameMode* _GameMode, APawn* _Pawn, AHUD* _HUD)
{
	GameMode = _GameMode;
	MainPawn = _Pawn;
	HUD = _HUD;
}

ULevel::~ULevel()
{
	BeginPlayList.clear();
	AllActorList.clear();
	Cameras.clear();
	while (false == PacketQueue.empty())
	{
		std::shared_ptr<UEngineProtocol> Packet = PacketQueue.front();
		PacketQueue.pop();
		Packet = nullptr;
	}
}