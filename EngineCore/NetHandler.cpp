#include "PreCompile.h"
#include "NetHandler.h"


UNetHandler::UNetHandler(UNetObject* _NetObject, AActor* _Actor)
{
	NetObject = _NetObject;
	Actor = _Actor;
}

UNetHandler::~UNetHandler()
{

}

