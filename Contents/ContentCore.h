#pragma once
#include <EngineCore/IContentsCore.h>

// 설명 : 윈도우 크기 설정, 리소스 로드, 레벨 생성
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

