#pragma once
#include <EngineBase/EngineMath.h>
#include "EngineInputLayOutInfo.h"

struct FEngineVertex
{
	friend class EngineVertexInit;
	__declspec(dllexport) static UEngineInputLayoutInfo Info;

	ENGINEAPI static UEngineInputLayoutInfo& GetInfo();

	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
};


