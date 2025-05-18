#include "PreCompile.h"
#include "NetObject.h"

UNetObject::UNetObject()
{
}

UNetObject::~UNetObject()
{
	//for (std::pair<const int, UNetObject*> Object : AllNetObjects)
	//{
	//	Object.second = nullptr;
	//}
	//AllNetObjects.clear();
}

void UNetObject::InitNetObject(int _ObjectToken, int _SessionToken)
{
	ObjectToken = _ObjectToken;
	SessionToken = _SessionToken;
	if (true == AllNetObjects.contains(ObjectToken))
	{
		MSGASSERT(_ObjectToken + "�� ������Ʈ ��ū �ߺ�");
		return;
	}
	
	//std::shared_ptr<UNetObject> Object = shared_from_this();
	AllNetObjects[_ObjectToken] = this;
}

bool UNetObject::IsNetObject(int _Token)
{
	return AllNetObjects.contains(_Token);
}

UNetObject* UNetObject::GetNetObject(int _Token)
{
	if (false == AllNetObjects.contains(_Token))
	{
		return nullptr;
	}

	return AllNetObjects[_Token];
}

