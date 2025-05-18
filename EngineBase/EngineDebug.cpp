#include "PreCompile.h"
#include "EngineDebug.h"

namespace UEngineDebug
{
	void CheckMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		
	}

	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}

	ENGINEAPI void StartConsole()
	{
		HWND hConsole = GetConsoleWindow();

		if (nullptr != hConsole)
		{
			return;
		}

		if (false == AllocConsole())
		{
			return;
		}

		hConsole = GetConsoleWindow();

		FILE* pCout = nullptr;
		FILE* pErr = nullptr;
		FILE* pCin = nullptr;
		errno_t Err;

		Err = freopen_s(&pCout, "CONOUT$", "w", stdout);
		if (0 == Err && pCout != nullptr && pCout != stdout)
		{
			fclose(pCout);
		}
		Err = freopen_s(&pCin, "CONIN$", "r", stdin);
		if (0 == Err && pCout != nullptr && pCin != stdin)
		{
			fclose(pCin);
		}

		Err = freopen_s(&pErr, "CONERR$", "w", stderr);
		if (0 == Err && pCout != nullptr && pErr != stderr)
		{
			fclose(pErr);
		}


		if (nullptr != hConsole)
		{
			int X = 1450;
			int Y = 0;
			int Width = 800;
			int Height = 400;
			MoveWindow(hConsole, X, Y, Width, Height, TRUE);
		}

		CONSOLE_FONT_INFOEX FontInfo = { 0 };
		FontInfo.cbSize = sizeof(FontInfo);
		FontInfo.nFont = 0;
		FontInfo.dwFontSize.X = 8;
		FontInfo.dwFontSize.Y = 16;
		FontInfo.FontFamily = FF_DONTCARE;
		FontInfo.FontWeight = FW_NORMAL;
		wcscpy_s(FontInfo.FaceName, L"Consolas");

		HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetCurrentConsoleFontEx(hConsoleHandle, FALSE, &FontInfo);
	}

	ENGINEAPI void EndConsole()
	{
		FreeConsole();
	}
}