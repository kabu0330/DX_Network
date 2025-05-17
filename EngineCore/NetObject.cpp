#include "PreCompile.h"
#include "NetObject.h"

UNetObject::UNetObject()
{
}

UNetObject::~UNetObject()
{
}

void UNetObject::InitNetObject(int _ObjectToken, int _SessionToken)
{
	ObjectToken = _ObjectToken;
	SessionToken = _SessionToken;
	if (true == AllNetObjects.contains(ObjectToken))
	{
		MSGASSERT(_ObjectToken + "번 오브젝트 토큰 중복");
		return;
	}
	
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

