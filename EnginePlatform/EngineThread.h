#pragma once

//#include <thread>
#include <functional>
#include <process.h> 

// ���� : 
class UEngineThread
{
public:
	ENGINEAPI UEngineThread();
	ENGINEAPI virtual ~UEngineThread();

	// delete Function
	UEngineThread(const UEngineThread& _Other) = delete;
	UEngineThread(UEngineThread&& _Other) noexcept = delete;
	UEngineThread& operator=(const UEngineThread& _Other) = delete;
	UEngineThread& operator=(UEngineThread&& _Other) noexcept = delete;

	ENGINEAPI bool Start(std::string _Name, std::function<void()> _Function);
	ENGINEAPI void Join();

    static void SetThreadNameDynamic(const std::string& _ThreadName);
protected:

private:
	std::string Name;
	//std::thread ThreadInst;
	std::function<void()> ThreadFunction;

	HANDLE ThreadHandle = nullptr;
	// CRT ������ ��Ʈ�� ����Ʈ
	static unsigned __stdcall ThreadEntry(void* _Param);

	//static void ThreadBaseFunction(UEngineThread* _Thread);
};

#pragma once

#if defined(_MSC_VER) && defined(_DEBUG)
#include <windows.h>

// MS_VC_EXCEPTION : Microsoft Visual C++ Exception  
static constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
// THREADNAME_INFO : Thread NAME INFO ����ü
struct THREADNAME_INFO
{
    DWORD dwType;       // Must be 0x1000 (����ü �ĺ���)
    LPCSTR szName;      // Long Pointer to Constant STRing : ������ �̸� (ASCII)
    DWORD dwThreadID;   // ������ ID (-1 �̸� ȣ�� ������)
    DWORD dwFlags;      // ����(Reserved), �ݵ�� 0
};
#pragma pack(pop)

inline void SetThreadName(
    DWORD _ThreadId,
    const char* _ThreadName
)
{
    THREADNAME_INFO Info;
    Info.dwType = 0x1000;
    Info.szName = _ThreadName;
    Info.dwThreadID = _ThreadId;
    Info.dwFlags = 0;

    __try
    {
        RaiseException(
            MS_VC_EXCEPTION,
            0,
            sizeof(Info) / sizeof(ULONG_PTR),
            reinterpret_cast<ULONG_PTR*>(&Info)
        );
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}
#endif

