#include "PreCompile.h"
#include "TransformObject.h"
#include "EngineCamera.h"

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> _Parent)
{
	SetupAttachment(_Parent.get());
}

void UTransformObject::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}

void UTransformObject::SetupAttachment(UTransformObject* _Parent)
{
	Parent = _Parent;
	Parent->Childs.push_back(this);

	TransformUpdate();
}

void UTransformObject::TransformUpdate()
{
	ParentMatrixCheck();
	Transform.TransformUpdate(IsAbsolute);

	for (UTransformObject* Child : Childs)
	{
		Child->TransformUpdate();
	}

	IsAbsolute = false;
}

void UTransformObject::CameraTransUpdate(UEngineCamera* _Camera)
{
	// 트랜스폼은 렌더러가 가지고 있다.
	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();
	// 랜더러는 월드 뷰 프로젝트를 다 세팅받았고
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;
}
