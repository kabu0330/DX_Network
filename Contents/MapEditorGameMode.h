#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class AMapEditorGameMode : public AGameMode 
{
public:
	// constrcuter destructer
	AMapEditorGameMode();
	~AMapEditorGameMode();

	// delete Function
	AMapEditorGameMode(const AMapEditorGameMode& _Other) = delete;
	AMapEditorGameMode(AMapEditorGameMode&& _Other) noexcept = delete;
	AMapEditorGameMode& operator=(const AMapEditorGameMode& _Other) = delete;
	AMapEditorGameMode& operator=(AMapEditorGameMode&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);
	void CheckInput(float _DeltaTime);

	void LevelChangeStart() override;

	static std::shared_ptr<class AMapObject>& GetMapObject()
	{
		return MapObject;
	}
protected:
	FVector NewTest = FVector::RIGHT;

private:
	std::shared_ptr<class MapEditorGUI> MapEditorWindow;
	static std::shared_ptr<class AMapObject> MapObject;
	std::shared_ptr<ACameraActor> Camera;
	std::shared_ptr<class USpriteRenderer> BackTexture;
};

