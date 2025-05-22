#include "PreCompile.h"
#include "EngineThread.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

UEngineThread::UEngineThread()
{
}

UEngineThread::~UEngineThread()
{
	Join();
}

bool UEngineThread::Start(std::string _Name, std::function<void()> _Function)
{
	if (nullptr != ThreadHandle)
	{
		return false;
	}

	Name = std::move(_Name);
	ThreadFunction = std::move(_Function);

	unsigned threadID = 0;
	ThreadHandle = reinterpret_cast<HANDLE>(
		::_beginthreadex(
			nullptr,
			0,
			ThreadEntry,
			this,
			0,
			&threadID
		)
	);

	if (nullptr == ThreadHandle)
	{
		return false;
	}


#if defined(_MSC_VER) && defined(_DEBUG)
	// ����ſ� ǥ���� ������ �̸� ����
	SetThreadName(::GetThreadId(ThreadHandle), Name.c_str());
#endif

	return true;

	//if (true == ThreadInst.joinable())
	//{
	//	return false;
	//}

	//if (nullptr == _Function)
	//{
	//	MSGASSERT("�Լ��� �������� ���� ������� start �Ҽ� �����ϴ�.");
	//	return false;
	//}

	//Name = _Name;
	//ThreadFunction = _Function;
	//ThreadInst = std::thread(std::bind(ThreadBaseFunction, this));
	//return true;
}

unsigned __stdcall UEngineThread::ThreadEntry(void* _Param)
{
	UEngineThread* Self = static_cast<UEngineThread*>(_Param);
	//std::wstring WName = UEngineString::AnsiToUnicode(Self->Name);
	//SetThreadDescription(GetCurrentThread(), WName.c_str());

	Self->ThreadFunction();

	::_endthreadex(0); // ���� ����
	return 0;
}

//void UEngineThread::ThreadBaseFunction(UEngineThread* _Thread)
//{
//	// ������ �۾��� ������
//	std::wstring WName = UEngineString::AnsiToUnicode(_Thread->Name);
//	SetThreadDescription(GetCurrentThread(), WName.c_str());
//
//	_Thread->ThreadFunction(); // Start()���� �Ѱ��� �Լ� ȣ��
//}

void UEngineThread::Join()
{
	if (nullptr != ThreadHandle)
	{
		::WaitForSingleObject(ThreadHandle, INFINITE);
		::CloseHandle(ThreadHandle);
		ThreadHandle = nullptr;
	}

	ThreadFunction = nullptr;
	//if (true == ThreadInst.joinable()) // �ڱ� �ڽ��� Join���� �ʵ��� ����ó��
	//{
	//	ThreadInst.join();
	//}
}

void UEngineThread::SetThreadNameDynamic(const std::string& _ThreadName)
{
	SetThreadName(::GetCurrentThreadId(), _ThreadName.c_str());
}

