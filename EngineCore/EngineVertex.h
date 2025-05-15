#pragma once
#include <EngineBase/EngineMath.h>
#include "EngineInputLayOutInfo.h"

// 설명 : 메시 렌더링을 위한 기본 정점 구조체
//		  위치나 컬러 뿐만 아니라 셰이더와 연결할 Inputlayout 정보도 함께 포함
struct FEngineVertex
{
	friend class EngineVertexInit;
	__declspec(dllexport) static UEngineInputLayoutInfo Info;

	ENGINEAPI static UEngineInputLayoutInfo& GetInfo();

	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
};


