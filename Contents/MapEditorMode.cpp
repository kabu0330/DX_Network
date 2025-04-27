#include "PreCompile.h"
#include "MapEditorMode.h"
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>

//std::shared_ptr<class ACameraActor> AMapEditorMode::Camera = nullptr;
//FVector AMapEditorMode::MousePos = { 0.0f, 0.0f, 0.0f };

class TestWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		GetMousePos();
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // �� ĭ ����
		ImGui::Text("test");


	}

	void GetMousePos()
	{
		//FVector MousePos = AMapEditorMode::MousePos;
		//ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);
	}
};

AMapEditorMode::AMapEditorMode()
{
	//Camera = GetWorld()->GetCamera(0);
	//MousePos = Camera->ScreenMousePosToWorldPos();

	UEngineGUI::CreateGUIWindow<TestWindow>("TestWindow");
}

void AMapEditorMode::BeginPlay()
{
	AActor::BeginPlay();

	Background = GetWorld()->SpawnActor<AMapObject>();
	Wall = GetWorld()->SpawnActor<AMapObject>();
	Wall->Create("MapObjectResources", { 10, 10 }, { 100, 100 });

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			Wall->SetTileIndex({ y, x }, { 0, -6 }, { 32, 44 }, 0);
		}
	}
}

void AMapEditorMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	DrawGrid(100);
	//MousePos = Camera->ScreenMousePosToWorldPos();

	//if (true == UEngineInput::IsPress(VK_LBUTTON))
	//{
	//	//Wall->SetTileLocation(MousePos, 1);
	//}

	//if (true == UEngineInput::IsPress(VK_RBUTTON))
	//{
	//	//std::shared_ptr<MapObjectData> Tile = Wall->GetTileRef(MousePos);
	//	if (nullptr != Tile->SpriteRenderer)
	//	{
	//		//Tile->SpriteRenderer->Destroy(5.0f);
	//		Tile->SpriteRenderer = nullptr;
	//	}
	//}
}

void AMapEditorMode::DrawGrid(int PixelInterval)
{
	//std::vector<VertexData> Vertices;
	//
	//FVector ScreenSize = UEngineCore::GetScreenScale();
	//int HalfWidth = ScreenSize.iX() / 2;
	//int HalfHeight = ScreenSize.iY() / 2;

	//// ����
	//for (int x = -HalfWidth; x < HalfWidth; x += PixelInterval)
	//{
	//	Vertices.push_back(VertexData{ float4({ static_cast<float>(x), static_cast<float>(HalfHeight), 0.0f }), // ��ġ
	//		float4({ 0.0f, 0.0f, 0.0f, 0.0f }), // UV
	//		float4({ 1.0f, 1.0f, 1.0f, 1.0f }) }); // ����
	//}

	//// ������
	//for (int y = -HalfHeight; y < HalfHeight; y += PixelInterval)
	//{
	//	Vertices.push_back(VertexData{ float4({  static_cast<float>(HalfWidth), static_cast<float>(y), 0.0f }), // ��ġ
	//		float4({ 0.0f, 0.0f, 0.0f, 0.0f }), // UV
	//		float4({ 1.0f, 1.0f, 1.0f, 1.0f }) }); // ����
	//}

	//D3D11_BUFFER_DESC BufferInfo = { 0, };
	//BufferInfo.ByteWidth = sizeof(VertexData) * static_cast<int>(Vertices.size());
	//BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // ���ؽ� ���۷� ���
	//BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU ���� ����
	//BufferInfo.Usage = D3D11_USAGE_DYNAMIC;             // ���� ���

	//D3D11_SUBRESOURCE_DATA Data; // ������ �����ּ� �־��ִ� ����ü
	//Data.pSysMem = &Vertices[0];

	//ID3D11Device* Device = UEngineCore::GetDevice().GetDevice();

	//HRESULT HR = Device->CreateBuffer(&BufferInfo, &Data, &VertexBuffer); // ù��°�� �����ϰ� �ι�°�� ����
	//if (S_OK != HR)
	//{
	//	MSGASSERT("���ؽ� ���� ������ �����߽��ϴ�.");
	//	return;
	//}

	//D3D11_MAPPED_SUBRESOURCE MapDesc = {};
	//ID3D11DeviceContext* Context = UEngineCore::GetDevice().GetContext();

	//HRESULT Result = Context->Map(VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MapDesc);
	//if (S_OK != Result)
	//{
	//	MSGASSERT("�׷���ī�� ���ο� �����߽��ϴ�.");
	//	return;
	//}
	//memcpy(MapDesc.pData, Vertices.data(), sizeof(VertexData) * Vertices.size());
	//Context->Unmap(VertexBuffer.Get(), 0);

	//UINT Stride = sizeof(VertexData);
	//UINT Offset = 0;
	//Context->IASetVertexBuffers(0, 2, &VertexBuffer, &Stride, &Offset);
	//Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//Context->Draw(static_cast<UINT>(Vertices.size()), 0);

}

AMapEditorMode::~AMapEditorMode()
{
}
