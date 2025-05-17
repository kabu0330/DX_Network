#pragma once
#include "EngineDefine.h"
#include <Windows.h>
#include <assert.h>


#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "ġ���� ����", MB_OK); assert(false);

#define ENGINEMSG(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "���", MB_OK);

namespace UEngineDebug
{
	ENGINEAPI void CheckMemoryLeak();

	ENGINEAPI void OutPutString(const std::string& Text);

	ENGINEAPI void StartConsole();

	ENGINEAPI void EndConsole();
}

