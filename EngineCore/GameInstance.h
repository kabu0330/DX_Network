#pragma once

// 설명 : 레벨의 생성과 파괴에 영향을 받지 않는 클래스
class UGameInstance
{
public:
	ENGINEAPI UGameInstance();
	ENGINEAPI virtual ~UGameInstance();

protected:

private:

private:
	// delete Function
	UGameInstance(const UGameInstance& _Other) = delete;
	UGameInstance(UGameInstance&& _Other) noexcept = delete;
	UGameInstance& operator=(const UGameInstance& _Other) = delete;
	UGameInstance& operator=(UGameInstance&& _Other) noexcept = delete;
};

