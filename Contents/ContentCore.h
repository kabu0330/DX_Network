#pragma once
#include <EngineCore/IContentsCore.h>

// ���� : ������ ũ�� ����, ���ҽ� �ε�, ���� ����
class UContentCore : public IContentsCore
{
public:
	UContentCore();
	~UContentCore();

protected:
	void EngineStart(UEngineInitData& _Data);
	void EngineTick(float _DeltaTime);
	void EngineEnd();

private:
	void SetWindowSize(UEngineInitData& _Data);

	void CreateLevel();
	void OpenLevel();

private:
	// delete Function
	UContentCore(const UContentCore& _Other) = delete;
	UContentCore(UContentCore&& _Other) noexcept = delete;
	UContentCore& operator=(const UContentCore& _Other) = delete;
	UContentCore& operator=(UContentCore&& _Other) noexcept = delete;

};

