#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AMapObject : public AActor
{
public:
	// constrcuter destructer
	AMapObject();
	~AMapObject();

	// delete Function
	AMapObject(const AMapObject& _Other) = delete;
	AMapObject(AMapObject&& _Other) noexcept = delete;
	AMapObject& operator=(const AMapObject& _Other) = delete;
	AMapObject& operator=(AMapObject&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


	void ChangeTexture(std::string_view _TextureName)
	{
		Renderer->SetTexture(_TextureName);
	}
	void SetSpriteIndex(int _Index)
	{
		Renderer->SetSprite("MapObjectResources", _Index);
		Index = _Index;
	}
	int GetIndex()
	{
		return Index;
	}
	std::shared_ptr<class USpriteRenderer> GetRenderer()
	{
		return Renderer;
	}

	EMapObjectType MapObjectTypeValue;

protected:
	std::shared_ptr<class USpriteRenderer> Renderer;
	int Index = 0;
private:


};

