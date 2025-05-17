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

	SaveFile(); // ���̺�
	ImGui::SameLine();
	LoadFile(); // �ε�
	ImGui::SameLine();
	LoadTexture();
	ImGui::SameLine();
	DeleteAllActors(); // ��ü ����

	SetScaleButton();
	SetRotationButton();
	SetActorLocationButton();

	// ���� ����Ʈ
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

		// �̹����� �������� ���� SRV�� �Ѱ��ش�.
		TextureSRV = reinterpret_cast<unsigned __int64>(Texture->GetSRV());

		if (ImGui::ImageButton(Name.c_str(), TextureSRV, { 60.0f, 60.0f }/* UV */))
		{
			// ���õ� �ؽ�ó�� ��������Ʈ ��ȣ ���
			SelectButtonIndex = i;
			
			// �̸������� �ؽ�ó �̹��� 
			SelectedTexture = TextureSRV;
			TextureScale = Texture->GetTextureSize();

			// Ŭ���� �ʱ�ȭ
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
	// IMGUI â ������ �̸����� â�� ���� ������ �ʵ��� ����ó��
	bool bIsHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
	bool bIsClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

	if (bIsHovered && !bIsClicked)
	{
		bIsPreview = false; // â �ȿ� ���� (ȣ����)
	}
	else
	{
		bIsPreview = true; // â �ۿ� ����
	}

	// ���콺���� �̸�����
	if (true == bIsPreview && 0 != SelectedTexture)
	{
		ImVec2 MousePos = ImGui::GetMousePos();
		ImVec2 Scale = { TextureScale.X * ScaleRatio, TextureScale.Y * ScaleRatio };
		ImVec2 HalfScale = { Scale.x / 2, Scale.y / 2 };

		// ���콺 ��ġ���� �̸����� �̹����� ���콺 ���߾ӿ� ������
		ImGui::SetNextWindowPos(ImVec2(MousePos.x - (Scale.x / 2), MousePos.y - (Scale.y / 2)));
		//ImGui::SetNextWindowPos(ImVec2(MousePos.x , MousePos.y ));

		// �̸����� â ����
		ImGui::Begin("Preview",
			nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoFocusOnAppearing);

		// ȸ��
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

		//// �簢���� �� ������ ��� (Z�� ȸ��)
		//ImVec2 P1 = RotatePointZ({ -HalfScale.x, -HalfScale.y }); // �»��
		//ImVec2 P2 = RotatePointZ({ HalfScale.x, -HalfScale.y });  // ����
		//ImVec2 P3 = RotatePointZ({ HalfScale.x, HalfScale.y });   // ���ϴ�
		//ImVec2 P4 = RotatePointZ({ -HalfScale.x, HalfScale.y });  // ���ϴ�

		//// ���콺 ��ġ�� �̵�
		//P1 = ImVec2(MousePos.x + P1.x, MousePos.y + P1.y);
		//P2 = ImVec2(MousePos.x + P2.x, MousePos.y + P2.y);
		//P3 = ImVec2(MousePos.x + P3.x, MousePos.y + P3.y);
		//P4 = ImVec2(MousePos.x + P4.x, MousePos.y + P4.y);

		//// ImDrawList�� ����� ȸ���� �ؽ�ó �׸���
		//ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
		//DrawList->AddImageQuad(
		//	SelectedTexture, // �ؽ�ó ID
		//	P1, P2, P3, P4,  // ȸ���� �� ��
		//	ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f) // UV ��ǥ
		//);


		ImGui::Image(SelectedTexture, Scale);
		ImGui::End();
	}

	// �̸����� ����� ����
	ImGuiStyle& style = ImGui::GetStyle();

	// â ������ ���� �������� ����
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

	static int LastSelectedItem = -1; // ������ ���õ� �׸� ���� ����

	if (0 < Arr.size())
	{
		ImGui::PushItemWidth(450);
		ImGui::ListBox("AllActorList", &ObjectItem, &Arr[0], static_cast<int>(Arr.size()));

		//AllMonsterList[SelectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f};

		// ���õ� �׸��� �ٲ���� ���
		if (LastSelectedItem != ObjectItem)
		{
			// ������ ���õ� ��ü�� ���� ����
			if (LastSelectedItem != -1)
			{
				AllMonsterList[LastSelectedItem]->GetRenderer()->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �⺻ ����
			}

			// ���õ� ��ü�� ���� ����
			if (ObjectItem != -1)
			{
				AllMonsterList[ObjectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f}; // �ʷϻ�

			}

			// ���� ���� ������Ʈ
			LastSelectedItem = ObjectItem;
		}


		if (ObjectItem != -1)
		{
			if (true == ImGui::Button("Pos Update"))
			{
				// ���õ� ��ü�� ��ġ ����
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
				AllMonsterList[LastSelectedItem]->GetRenderer()->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �⺻ ����
				XValue = 0;
				YValue = 0;
				ObjectItem = -1;
				LastSelectedItem = -1;
		
			}

			// ���� ��ư Ȱ��ȭ
			// AllMonsterList[ObjectItem]->
			if (true == ImGui::Button("Delete"))
			{
				AllMonsterList[ObjectItem]->Destroy();

				ObjectItem = -1;
				LastSelectedItem = -1; // ���� ���� �ʱ�ȭ
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

		// ������ ���?
		//FVector Scale = AMapEditorGameMode::GetMapObject()->GetRenderer()->GetScale();


		AMapEditorGameMode::GetMapObject()->GetRenderer()->SetAutoScaleRatio(ScaleRatio);
		//AMapEditorGameMode::GetMapObject()->GetRenderer()->SetRotation(RotationValue);
		AMapEditorGameMode::GetMapObject()->SetActorLocation(Pos);

		// �̸����� ����
		bIsPreview = true;

	}
}

void MapEditorGUI::SaveFile()
{
	if (true == ImGui::Button("Save"))
	{
		// 1. ������ ���� ��ġ �����ϰ�
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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

			// ������ �������� ũ�⸸ŭ 
			Ser << static_cast<int>(AllMapObjectList.size());

			for (std::shared_ptr<AMapObject> Actor : AllMapObjectList)
			{
				// ���⼭ ������ �� �ִ� �����ʹ� ���⼭ �����ϰ�
				Ser << static_cast<int>(Actor->MapObjectTypeValue); 

				// Actor�� ������ �ִ� �����ʹ� Ŭ������ Serialize���� �����Ѵ�.
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
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
				
				// ���⼭ ���� �����ʹ� ���� ������ ����
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

				// �������� ���⼭ ������ȭ�� �Ѵ�.
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
