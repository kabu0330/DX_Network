#include <../EngineShader/RenderBaseData.hlsli>

struct FEngineVertex
{
	float4 POSITION : POSITION;
	float4 UV : TEXCOORD;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION; // ����Ʈ����� �������� �������Դϴ�.
	float4 UV : TEXCOORD; // 
};

cbuffer MyData : register(b9)
{
	float4 ScreenScale;
};

// ���ؽ����̴��� �� �������.
VertexShaderOutPut JHJEffect_VS(FEngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.UV = _Vertex.UV;
	OutPut.SVPOSITION = _Vertex.POSITION;
	
	OutPut.UV.x -= 0.5f;
	OutPut.UV.y -= 0.5f;
	
	// -1.5 -1.5 1.5 -1.5
	// -1.5 1.5  1.5 1.5
	
	float ATime = AccTime * AccTime;
	
	if (0.0f > OutPut.UV.x)
	{
		OutPut.UV.x -= ATime + 1.0f;
	}
	
	if (0.5f < OutPut.UV.x)
	{
		OutPut.UV.x += ATime + 1.0f;
	}
	
	if (0.0f > OutPut.UV.y)
	{
		OutPut.UV.y -= ATime + 1.0f;
	}
	
	if (0.5f < OutPut.UV.y)
	{
		OutPut.UV.y += ATime + 1.0f;
	}
	
	
	return OutPut;
}


Texture2D ImageTexture : register(t1);
SamplerState ImageSampler : register(s1);

// �̹����� ���ø��ؼ� �̹����� ���̰� �����
float4 JHJEffect_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	// _Vertex.SVPOSITION;
	// 00  
	//         1280 720
	
	if (_Vertex.UV.x < 0)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	if (_Vertex.UV.y < 0)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	if (_Vertex.UV.x > 1.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	if (_Vertex.UV.y > 1.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	float4 Color = ImageTexture.Sample(ImageSampler, _Vertex.UV.xy);
	return Color;
};
