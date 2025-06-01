#pragma once

// 설명 : 크기와 위치를 설정할 수 있는 클래스
class UTransformObject 
{
public:
	ENGINEAPI UTransformObject();
	ENGINEAPI virtual ~UTransformObject();

	ENGINEAPI virtual void CameraTransUpdate(class UEngineCamera* _Camera);

	ENGINEAPI void SetupAttachment(std::shared_ptr<UTransformObject> _Parent);

	ENGINEAPI void SetupAttachment(UTransformObject* _Parent);

	ENGINEAPI void TransformUpdate();

	// 이동
	void AddRelativeLocation(const FVector& _Value)
	{
		Transform.Location += _Value;
		TransformUpdate();
	}
	void AddWorldLocation(const FVector& _Value)
	{
		bIsAbsolute = true;
		Transform.Location += _Value;
		TransformUpdate();
	}

	void SetRelativeLocation(const FVector& _Value)
	{
		Transform.Location = _Value;
		TransformUpdate();
	}
	void SetActorLocation(const FVector& _Value)
	{
		bIsAbsolute = true;
		Transform.Location = _Value;
		TransformUpdate();
	}

	FVector GetRelativeLocation()
	{
		return Transform.RelativeLocation;
	}

	FVector GetWorldLocation()
	{
		return Transform.WorldLocation;
	}


	// 회전
	void AddLocalRotation(const FVector& _Value)
	{
		Transform.Rotation += _Value;
		TransformUpdate();
	}
	void AddWorldRotation(const FVector& _Value)
	{
		bIsAbsolute = true;
		Transform.Rotation += _Value;
		TransformUpdate();
	}

	void SetLocalRotation(const FVector& _Value)
	{
		Transform.Rotation = _Value;
		TransformUpdate();
	}


	// 크기
	void AddRelativeScale3D(const FVector& _Value)
	{
		Transform.Scale += _Value;
		Transform.Scale.W = 0.0f;
		TransformUpdate();
	}
	void AddScale3D(const FVector& _Value)
	{
		bIsAbsolute = true;
		Transform.Scale += _Value;
		TransformUpdate();
	}

	void SetRelativeScale3D(const FVector& _Value)
	{
		Transform.Scale = _Value;
		Transform.Scale.W = 0.0f;
		TransformUpdate();
	}
	void SetScale3D(const FVector& _Value)
	{
		bIsAbsolute = true;
		Transform.Scale = _Value;
		TransformUpdate();
	}

	FVector GetWorldScale3D()
	{
		return Transform.WorldScale;
	}
	FVector GetRelativeScale3D()
	{
		return Transform.Scale;
	}


	FTransform& GetTransformRef()
	{
		return Transform;
	}

	UTransformObject* Parent = nullptr;

protected:
	void CheckParentMatrix();

	FTransform Transform;
	std::list<UTransformObject*> Children;

	bool bIsAbsolute = false;

private:
	// delete Function
	UTransformObject(const UTransformObject& _Other) = delete;
	UTransformObject(UTransformObject&& _Other) noexcept = delete;
	UTransformObject& operator=(const UTransformObject& _Other) = delete;
	UTransformObject& operator=(UTransformObject&& _Other) noexcept = delete;
};

