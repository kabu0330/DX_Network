#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"
#include <set>

// 설명 : 
class UCollision : public USceneComponent
{
public:
	// constrcuter destructer
	ENGINEAPI UCollision();
	ENGINEAPI ~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	ENGINEAPI void SetCollisionProfileName(std::string_view _ProfileName);
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void DebugRender(UEngineCamera* _Camera, float _DeltaTime);

	std::string GetCollisionProfileName()
	{
		return ProfileName;
	}

	void SetRadius(float _Value);

	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, std::vector<UCollision*>& _Vector);

	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, FVector _NextPos, std::vector<UCollision*>& _Vector);

	void SetCollisionType(ECollisionType _Type)
	{
		CollisionType = _Type;
	}

	void CollisionEventCheck(std::shared_ptr<UCollision> _Other);

	ENGINEAPI void SetCollisionEnter(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionStay(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionEnd(std::function<void(UCollision*, UCollision*)> _Function);

	bool IsEvent()
	{
		return Enter != nullptr || Stay != nullptr || End != nullptr;
	}

	void SetDebugColor(const FVector& _Color)
	{
		DebugColor = _Color;
	}
	FVector GetDebugColor() const
	{
		return DebugColor;
	}

	ENGINEAPI static bool& GetDebugModeRef();

private:
	static bool bIsDebugMode;
	FVector DebugColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	ECollisionType CollisionType = ECollisionType::OBB2D;
	// 내가 충돌한 상대를 기억하는 용도의 set
	std::set<UCollision*> CollisionCheckSet;

	// 나는 Monster 그룹이다.
	std::string ProfileName = "NONE";

	// 최초 충돌
	std::function<void(UCollision*, UCollision*)> Enter;

	// 지속 충돌 
	std::function<void(UCollision*, UCollision*)> Stay;

	// 충돌 끝.
	std::function<void(UCollision*, UCollision*)> End;
};

