#include "PreCompile.h"
#include "EngineVertex.h"

UEngineInputLayoutInfo FEngineVertex::Info;

UEngineInputLayoutInfo& FEngineVertex::GetInfo()
{
	return Info;
}

// 설명 : FEngineVertex에 대한 InputLayout 초기화를 수행하는 클래스
//		  DX 셰이더와 자동 연결되도록 내부 Info에 레이아웃 정보 등록
class EngineVertexInit
{
public:
	ENGINEAPI EngineVertexInit()
	{
		FEngineVertex::Info.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
		FEngineVertex::Info.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
		FEngineVertex::Info.AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
};

EngineVertexInit InitObject; 