#pragma once

#define USEDLL

#ifdef USEDLL
#define ENGINEAPI __declspec(dllexport)
#else
#define ENGINEAPI __declspec(dllimport)
#endif