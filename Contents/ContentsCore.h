#pragma once
#include <EngineCore/IContentsCore.h>

// ���� : ������ ũ�� ����, ���ҽ� �ε�, ���� ����
class UContentsCore : public IContentsCore
{
public:
	UContentsCore();
	~UContentsCore();

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
	UContentsCore(const UContentsCore& _Other) = delete;
	UContentsCore(UContentsCore&& _Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& _Other) = delete;
	UContentsCore& operator=(UContentsCore&& _Other) noexcept = delete;

};

