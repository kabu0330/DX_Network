#pragma once
#include <EngineCore/GameMode.h>
#include "MapObject.h"

struct VertexData
{
	float4 Position;
	float4 TexCoord;
	float4 Color;
};

// Ό³Έν :
class AMapEditorMode : public AGameMode
{
public:
	// constrcuter destructer
	AMapEditorMode();
	~AMapEditorMode();

	// delete Function
	AMapEditorMode(const AMapEditorMode& _Other) = delete;
	AMapEditorMode(AMapEditorMode&& _Other) noexcept = delete;
	AMapEditorMode& operator=(const AMapEditorMode& _Other) = delete;
	AMapEditorMode& operator=(AMapEditorMode&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DrawGrid(int PixelInterval);


	//static FVector MousePos;
protected:

private:
	//static std::shared_ptr<class ACameraActor> Camera;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UEngineInputLayOutInfo* InfoPtr;

	std::shared_ptr<AMapObject> Background = nullptr;
	std::shared_ptr<AMapObject> Wall = nullptr;
};

