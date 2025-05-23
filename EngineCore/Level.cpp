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
	SpawnCamera(EEngineCameraType::MAIN_CAMERA); // 메인카메라

	std::shared_ptr<ACameraActor> UI_CAMERA = SpawnCamera(EEngineCameraType::UI_CAMERA); // UI 카메라
	UI_CAMERA->GetCameraComponent()->SetProjectionType(EProjectionType::ORTHOGRAPHIC); // UI카메라는 원근투영 안한다.

	// 화면에 그려질 최종 렌더타겟을 만든다.
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

	if (GetMainCamera()->IsFreeCamera()) // 프리 카메라를 켜면 액터의 Tick은 일시정지
	{
		return;
	}

	// Tick을 돌리기 전에, 해당 프레임에 생성된 Actor의 BeginPlay를 먼저 호출한다.
	std::list<std::shared_ptr<class AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<class AActor>>::iterator EndIter = BeginPlayList.end();
	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive()) // 비활성화된 액터는 일단 생략
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter); 
		CurActor->BeginPlay(); // 호출

		if (nullptr != CurActor->Parent)
		{
			continue;
		}

		AllActorList.push_back(CurActor); // BeginPlay가 호출된 Actor만 Tick이 도는 ActorList에 들어간다.
	}

	// BeginPlay가 호출된 Actor
	for (std::shared_ptr<AActor> CurActor : AllActorList)
	{
		if (false == CurActor->IsActive()) // 비활성화된 Actor는 Tick을 돌리지 않는다.
		{
			continue;
		}

		CurActor->Tick(_DeltaTime);
	}
}

void ULevel::Render(float _DeltaTime)
{
	UEngineCore::GetDevice().OMSetRenderTargetWithClear(); // 백버퍼 초기화 및 OM단계에서 사용할 RTV와 DSV 설정

	LastRenderTarget->ClearRenderTargets(); // 최종 출력 화면도 화면 한 번 지워

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		if (Camera.first == static_cast<int>(EEngineCameraType::UI_CAMERA)) 	// UI카메라는 따로 렌더를 돌려준다.
		{
			continue;
		}

		if (false == Camera.second->IsActive())
		{
			continue;
		}

		Camera.second->Tick(_DeltaTime); // View 행렬과 Projection 행렬 계산
		Camera.second->GetCameraComponent()->Render(_DeltaTime);

		// 특정 카메라만 포스트 이펙트
		 //Camera.second->ShowPostEffect();

		Camera.second->GetCameraComponent()->CameraTarget->MergeTo(LastRenderTarget); // 렌더링 파이프라인으로
	}

	// 여기서 하면 화면 전체 포스트 이펙트 적용
	//LastRenderTarget->ShowPostEffect(); 

	if (true == Cameras.contains(static_cast<int>(EEngineCameraType::UI_CAMERA))) // UI카메라는 따로 렌더를 돌려준다.
	{
		std::shared_ptr<ACameraActor> CameraActor = Cameras[static_cast<int>(EEngineCameraType::UI_CAMERA)];
		if (true == CameraActor->IsActive()) // UI카메라가 액티브 상태일때만 돌린다.
		{
			std::shared_ptr<UEngineCamera> CameraComponent = Cameras[static_cast<int>(EEngineCameraType::UI_CAMERA)]->GetCameraComponent();

			CameraActor->Tick(_DeltaTime); 

			CameraComponent->CameraTarget->ClearRenderTargets(); // 화면도 지우고
			CameraComponent->CameraTarget->OMSetRenderTargets(); // 렌더타겟 세팅하고

			HUD->UIRender(CameraComponent.get(), _DeltaTime); // 위젯의 틱, 렌더 돌리고

			CameraComponent->CameraTarget->MergeTo(LastRenderTarget); // 최종 출력병합하고
		}
	}
	else
	{
		MSGASSERT("UI카메라가 존재하지 않습니다. 엔진 오류입니다. UI카메라를 제작해주세요.");
		return;
	}

	// 백버퍼 렌더 타겟 출력 병합
	std::shared_ptr<UEngineRenderTarget> BackBuffer = UEngineCore::GetDevice().GetBackBufferTarget();
	LastRenderTarget->MergeTo(BackBuffer);

	// 디버그 렌더링 : 주로 콜리전
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

				_Collision->DebugRender(Camera->GetCameraComponent().get(), _DeltaTime); // 디버그
			}
		}
	}

	// IMGUI 랜더링
	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this, _DeltaTime);
	}

	UEngineCore::GetDevice().Present(); // 스왑체인이 관리하는 백버퍼와 프론트버퍼를 교환(Swap) 
	// 프론트버퍼(윈도우 창)에 출력
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

				// 컴포넌트의 메모리를 삭제할 수 있는 권한은 오로지 액터만 가지고 있다.
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

			// 이제 delete도 필요 없다.
			StartIter = List.erase(StartIter);
		}
	}
}

void ULevel::ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	if (false == Cameras.contains(_CameraOrder))
	{
		MSGASSERT("존재하지 않는 카메라에 랜더러를 집어넣으려고 했습니다.");
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
		MSGASSERT("이미 존재하는 카메라 Order입니다. 다른 값을 입력해주세요. : " + std::to_string(_Order));
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
		MSGASSERT("존재하지 않는 콜리전 그룹에 랜더러를 집어넣으려고 했습니다.");
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