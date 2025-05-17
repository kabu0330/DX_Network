#include "PreCompile.h"
#include "MapEditorGUI.h"

#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EngineCore/EngineCamera.h>

#include "MapObject.h"
#include "MapEditorGameMode.h"

enum class ESpawnList
{
	Background,
	Object,
};

void MapEditorGUI::OnGUI(float _DelataTime)
{
	//float DeltaTime = UEngineCore::GetDeltaTime();
	//std::string DeltaTimeText = "FPS : " + std::to_string(static_cast<int>(1.0f / DeltaTime));
	//ImGui::Text(DeltaTimeText.c_str());

	Sprite = UEngineSprite::Find<UEngineSprite>("MapObjectResources");
	if (nullptr == Sprite)
	{
		return;
	}
	Texture = nullptr;
	TextureSRV = 0;
	std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetMainCamera();
	Pos = Camera->ScreenMousePosToWorldPos();

	SaveFile(); // 세이브
	ImGui::SameLine();
	LoadFile(); // 로드
	ImGui::SameLine();
	LoadTexture();
	ImGui::SameLine();
	DeleteAllActors(); // 전체 삭제

	SetScaleButton();
	SetRotationButton();
	SetActorLocationButton();

	// 스폰 리스트
	ShowActorListBox(); 


	ShowSpriteImageButton();

	SpawnActor();

	RenderPreview();
}

void MapEditorGUI::SetScaleButton()
{
	ImGui::PushItemWidth(50.0f);
	ImGui::InputFloat("Scale", &ScaleRatio);

	if (UEngineInput::IsDown('Q'))
	{
		ScaleRatio += 0.1f;
	}
	if (UEngineInput::IsDown('E'))
	{
		ScaleRatio -= 0.1f;
	}
	if (UEngineInput::IsDown('R'))
	{
		ScaleRatio = 1.0f;
	}
}

void MapEditorGUI::SetRotationButton()
{
	float RotationZ = RotationValue.Z;
	ImGui::SliderFloat("Z Angle ", &RotationZ, 0.0f, 360.0f);
	ImGui::SameLine();
	float RotationY = RotationValue.Y;
	ImGui::SliderFloat("Y Angle ", &RotationY, 0.0f, 360.0f);

	if (UEngineInput::IsDown('1'))
	{
		RotationValue.Z += 90.0f;
	}
	if (UEngineInput::IsDown('2'))
	{
		RotationValue.Z -= 90.0f;
	}

	if (UEngineInput::IsDown('Z'))
	{
		RotationValue.Z += 15.0f;
	}
	if (UEngineInput::IsDown('X'))
	{
		RotationValue.Z -= 15.0f;
	}
	if (UEngineInput::IsDown('C'))
	{
		if (180.0f <= RotationValue.Y)
		{
			RotationValue.Y = 0.0f;
			return;
		}

		RotationValue.Y += 180.0f;
	}
	if (UEngineInput::IsDown('V'))
	{
		RotationValue = FVector::ZERO;
	}
}

void MapEditorGUI::SetActorLocationButton()
{
	float Gap = 30.0f;
	float WidthGap = 120.0f;
	float ItemWidth = 100.0f;

	ImGui::Columns(6, nullptr, false);

	ImGui::SetColumnWidth(0, Gap);
	ImGui::Text("X :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(1, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#X", &XValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::SetColumnWidth(2, Gap);
	ImGui::Text("Y :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(3, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#Y", &YValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::SetColumnWidth(4, Gap);
	ImGui::Text("Z :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(5, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#Z", &ZValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void MapEditorGUI::ShowSpriteImageButton()
{
	ImVec2 ChildSize = { 550.0f, 500.0f };
	ImGui::BeginChild("Scroll", ChildSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for (int i = 0; i < Sprite->GetSpriteCount(); i++)
	{
		std::string Name = std::to_string(i);
		
		Texture = Sprite->GetTexture(i);

		// 이미지를 가져오기 위해 SRV를 넘겨준다.
		TextureSRV = reinterpret_cast<unsigned __int64>(Texture->GetSRV());

		if (ImGui::ImageButton(Name.c_str(), TextureSRV, { 60.0f, 60.0f }/* UV */))
		{
			// 선택된 텍스처의 스프라이트 번호 기억
			SelectButtonIndex = i;
			
			// 미리보기할 텍스처 이미지 
			SelectedTexture = TextureSRV;
			TextureScale = Texture->GetTextureSize();

			// 클릭시 초기화
			bIsPreview = true;
			ScaleRatio = 1.0f;
			RotationValue = FVector::ZERO;
			int a = 0;
		}

		int SameLineIndex = 7;
		if (0 == i)
		{
			i += 1;
		}
		if (i != Sprite->GetSpriteCount() - 1 && i % SameLineIndex)
		{
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
}

void MapEditorGUI::RenderPreview()
{
	// IMGUI 창 내에도 미리보기 창이 따라 들어오지 않도록 예외처리
	bool bIsHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
	bool bIsClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

	if (bIsHovered && !bIsClicked)
	{
		bIsPreview = false; // 창 안에 있음 (호버만)
	}
	else
	{
		bIsPreview = true; // 창 밖에 있음
	}

	// 마우스에서 미리보기
	if (true == bIsPreview && 0 != SelectedTexture)
	{
		ImVec2 MousePos = ImGui::GetMousePos();
		ImVec2 Scale = { TextureScale.X * ScaleRatio, TextureScale.Y * ScaleRatio };
		ImVec2 HalfScale = { Scale.x / 2, Scale.y / 2 };

		// 마우스 위치에서 미리보기 이미지가 마우스 정중앙에 오도록
		ImGui::SetNextWindowPos(ImVec2(MousePos.x - (Scale.x / 2), MousePos.y - (Scale.y / 2)));
		//ImGui::SetNextWindowPos(ImVec2(MousePos.x , MousePos.y ));

		// 미리보기 창 생성
		ImGui::Begin("Preview",
			nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoFocusOnAppearing);

		// 회전
		//FVector Radians = FVector::ZERO;
		//Radians.Y = RotationValue.Y * UEngineMath::D2R;
		//Radians.Z = RotationValue.Z * UEngineMath::D2R;

		//float CosZ = cos(Radians.Z);
		//float SinZ = sin(Radians.Z);

		//auto RotatePointZ = [&](const ImVec2& point) -> ImVec2 {
		//	return ImVec2(
		//		CosZ * point.x - CosZ * point.y,
		//		SinZ * point.x + SinZ * point.y
		//	);
		//	};

		//// 사각형의 네 꼭짓점 계산 (Z축 회전)
		//ImVec2 P1 = RotatePointZ({ -HalfScale.x, -HalfScale.y }); // 좌상단
		//ImVec2 P2 = RotatePointZ({ HalfScale.x, -HalfScale.y });  // 우상단
		//ImVec2 P3 = RotatePointZ({ HalfScale.x, HalfScale.y });   // 우하단
		//ImVec2 P4 = RotatePointZ({ -HalfScale.x, HalfScale.y });  // 좌하단

		//// 마우스 위치로 이동
		//P1 = ImVec2(MousePos.x + P1.x, MousePos.y + P1.y);
		//P2 = ImVec2(MousePos.x + P2.x, MousePos.y + P2.y);
		//P3 = ImVec2(MousePos.x + P3.x, MousePos.y + P3.y);
		//P4 = ImVec2(MousePos.x + P4.x, MousePos.y + P4.y);

		//// ImDrawList를 사용해 회전된 텍스처 그리기
		//ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
		//DrawList->AddImageQuad(
		//	SelectedTexture, // 텍스처 ID
		//	P1, P2, P3, P4,  // 회전된 네 점
		//	ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f) // UV 좌표
		//);


		ImGui::Image(SelectedTexture, Scale);
		ImGui::End();
	}

	// 미리보기 투명색 설정
	ImGuiStyle& style = ImGui::GetStyle();

	// 창 배경색을 완전 투명으로 설정
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // RGBA
}

void MapEditorGUI::ShowActorListBox()
{
	std::vector<std::shared_ptr<AMapObject>> AllMonsterList = GetWorld()->GetAllActorArrayByClass<AMapObject>();

	std::vector<std::string> ArrString;
	for (std::shared_ptr<class AActor> Actor : AllMonsterList)
	{
		std::string Text = "";
		Text += Actor->GetName() + " ";
		Text += Actor->GetActorLocation().ToString();

		ArrString.push_back(Text);
	}

	std::vector<const char*> Arr;
	for (size_t i = 0; i < ArrString.size(); i++)
	{
		Arr.push_back(ArrString[i].c_str());
	}

	static int LastSelectedItem = -1; // 이전에 선택된 항목 추적 변수

	if (0 < Arr.size())
	{
		ImGui::PushItemWidth(450);
		ImGui::ListBox("AllActorList", &ObjectItem, &Arr[0], static_cast<int>(Arr.size()));

		//AllMonsterList[SelectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f};

		// 선택된 항목이 바뀌었을 경우
		if (LastSelectedItem != ObjectItem)
		{
			// 이전에 선택된 객체의 색상 복원
			if (LastSelectedItem != -1)
			{
				AllMonsterList[LastSelectedItem]->GetRenderer()->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // 기본 색상
			}

			// 선택된 객체의 색상 변경
			if (ObjectItem != -1)
			{
				AllMonsterList[ObjectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f}; // 초록색

			}

			// 선택 상태 업데이트
			LastSelectedItem = ObjectItem;
		}


		if (ObjectItem != -1)
		{
			if (true == ImGui::Button("Pos Update"))
			{
				// 선택된 객체의 위치 변경
				FVector Pos = AllMonsterList[ObjectItem]->GetActorLocation();
				if (0 != XValue)
				{
					Pos.X = static_cast<float>(XValue);
					AllMonsterList[ObjectItem]->SetActorLocation(Pos);
				}
				if (0 != YValue)
				{
					Pos.Y = static_cast<float>(YValue);
					AllMonsterList[ObjectItem]->SetActorLocation(Pos);
				}
			}

			if (true == ImGui::Button("Select Delete"))
			{
				AllMonsterList[LastSelectedItem]->GetRenderer()->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // 기본 색상
				XValue = 0;
				YValue = 0;
				ObjectItem = -1;
				LastSelectedItem = -1;
		
			}

			// 삭제 버튼 활성화
			// AllMonsterList[ObjectItem]->
			if (true == ImGui::Button("Delete"))
			{
				AllMonsterList[ObjectItem]->Destroy();

				ObjectItem = -1;
				LastSelectedItem = -1; // 선택 상태 초기화
			}
		}
	}
}

void MapEditorGUI::DeleteAllActors()
{
	if (ImGui::Button("All Delete"))
	{
		std::list<std::shared_ptr<AMapObject>> AllList = GetWorld()->GetAllActorListByClass<AMapObject>();
		for (std::shared_ptr<AMapObject> Actor : AllList)
		{
			Actor->Destroy();
		}
	}
}

void MapEditorGUI::SpawnActor()
{
	if (true == UEngineInput::IsDown(VK_RBUTTON))
	{
		ESpawnList Select = static_cast<ESpawnList>(SelectItem);

		//FVector Pos = Camera->ScreenMousePosToWorldPos();
		if (0 != XValue)
		{
			Pos.X = static_cast<float>(XValue);
		}
		if (0 != YValue)
		{
			Pos.Y = static_cast<float>(YValue);
		}

		ZValue -= 1;
		Pos.Z = static_cast<float>(ZValue);

		//switch (Select)
		//{
		//case ESpawnList::Background:
		//	AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
		//	break;
		//case ESpawnList::Object:
		//	AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
		//	break;
		//default:
		//	break;
		//}

		AMapEditorGameMode::GetMapObject()->SetSpriteIndex(SelectButtonIndex);

		// 스케일 어떻게?
		//FVector Scale = AMapEditorGameMode::GetMapObject()->GetRenderer()->GetScale();


		AMapEditorGameMode::GetMapObject()->GetRenderer()->SetAutoScaleRatio(ScaleRatio);
		//AMapEditorGameMode::GetMapObject()->GetRenderer()->SetRotation(RotationValue);
		AMapEditorGameMode::GetMapObject()->SetActorLocation(Pos);

		// 미리보기 유지
		bIsPreview = true;

	}
}

void MapEditorGUI::SaveFile()
{
	if (true == ImGui::Button("Save"))
	{
		// 1. 저장할 폴더 위치 지정하고
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("MapData");
		std::string InitPath = Dir.GetPathToString();

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260] = { 0 };       // if using TCHAR macros
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = ("All\0*.*\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrDefExt = "MapData";
		ofn.lpstrInitialDir = InitPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			std::list<std::shared_ptr<AMapObject>> AllMapObjectList = GetWorld()->GetAllActorListByClass<AMapObject>();

			UEngineSerializer Ser;

			// 저장할 데이터의 크기만큼 
			Ser << static_cast<int>(AllMapObjectList.size());

			for (std::shared_ptr<AMapObject> Actor : AllMapObjectList)
			{
				// 여기서 저장할 수 있는 데이터는 여기서 저장하고
				Ser << static_cast<int>(Actor->MapObjectTypeValue); 

				// Actor가 가지고 있는 데이터는 클래스의 Serialize에서 저장한다.
				Actor->Serialize(Ser);
			}

			UEngineFile NewFile = Dir.GetFile(ofn.lpstrFile);

			NewFile.FileOpen("wb");
			NewFile.Write(Ser);
		}
	}
}

void MapEditorGUI::LoadFile()
{
	if (true == ImGui::Button("LoadTexture"))
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("MapData");
		std::string InitPath = Dir.GetPathToString();

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260] = { 0 };       // if using TCHAR macros
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = ("All\0*.*\0Text\0*.MapData\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = InitPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			UEngineFile NewFile = Dir.GetFile(ofn.lpstrFile);
			UEngineSerializer Ser;

			NewFile.FileOpen("rb");
			NewFile.Read(Ser);

			int Count = 0;

			Ser >> Count;

			for (size_t i = 0; i < Count; i++)
			{
				int TypeValue = 0;
				int Index = -1;
				
				// 여기서 꺼낼 데이터는 먼저 꺼내서 쓰고
				Ser >> TypeValue;

				EMapObjectType ObjectType = static_cast<EMapObjectType>(TypeValue);


				switch (ObjectType)
				{
				//case BACKGROUND_COLOR:
				//	AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>();
				//	break;
				//case BACKGROUND_OBJECT:
				//	AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>();
					break;
				case COLLISION_OBJECT:
					break;
				case NONE_COLLISION_OBJECT:
					break;
				case MAX_OBJECT_TYPE:
					break;
				default:
					break;
				}

				// 나머지는 여기서 역직렬화를 한다.
				AMapEditorGameMode::GetMapObject()->DeSerialize(Ser);
			}
		}
	}
}

void MapEditorGUI::LoadTexture()
{

}

MapEditorGUI::MapEditorGUI()
{
}

MapEditorGUI::~MapEditorGUI()
{
}
