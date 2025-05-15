#pragma once

typedef __int8 int8;

enum class EProjectionType : int8
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};

enum class EEngineCameraType : int8
{
	MAIN_CAMERA,
	UI_CAMERA = 127,
};

enum class EShaderType : int8
{
	VS, 
	HS, 
	DS,
	GS,
	PS,
	CS,
	MAX_SHADER_TYPE
};
