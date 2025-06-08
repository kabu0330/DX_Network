#include "PreCompile.h"
#include "NetObject.h"
#include "NetHandler.h"

UNetObject::UNetObject()
{
}

UNetObject::~UNetObject()
{
	//for (std::pair<const int, UNetObject*> Object : AllNetObjects)
	//{
	//	Object.second = nullptr;
	//}
	AllNetObjects.clear();
}

void UNetObject::CreateNetObjectSetting(int _ObjectToken, int _SessionToken)
{
	ObjectToken = _ObjectToken;
	SessionToken = _SessionToken;
	if (true == AllNetObjects.contains(ObjectToken))
	{
		MSGASSERT(_ObjectToken + "�� ������Ʈ ��ū �ߺ�");
		return;
	}
	
	AllNetObjects[_ObjectToken] = this;
	std::cout << "������Ʈ ��ū : " << _ObjectToken << ", ���� ��ū : " << _SessionToken << std::endl;
}

void UNetObject::InitActorData(AActor* _Actor)
{
	NetHandler = std::make_shared<UNetHandler>(this, _Actor);
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

