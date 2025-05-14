#include "PreCompile.h"
#include "TransformObject.h"
#include "EngineCamera.h"

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> _Parent)
{
	SetupAttachment(_Parent.get());
}

void UTransformObject::SetupAttachment(UTransformObject* _Parent)
{
	Parent = _Parent;
	Parent->Children.push_back(this);

	TransformUpdate();
}

void UTransformObject::TransformUpdate()
{
	CheckParentMatrix();
	Transform.TransformUpdate(bIsAbsolute);

	for (UTransformObject* Child : Children)
	{
		Child->TransformUpdate();
	}

	bIsAbsolute = false;
}

void UTransformObject::CheckParentMatrix()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}

void UTransformObject::CameraTransUpdate(UEngineCamera* _Camera)
{
	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef(); // �������� Ʈ������

	// �������� ���� �� ������Ʈ�� �� ���ù޾Ұ�
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;

	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;
}

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}