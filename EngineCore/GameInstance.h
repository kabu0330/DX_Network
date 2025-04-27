#pragma once

// Ό³Έν :
class UGameInstance
{
public:
	// constrcuter destructer
	ENGINEAPI UGameInstance();
	ENGINEAPI virtual ~UGameInstance();

	// delete Function
	UGameInstance(const UGameInstance& _Other) = delete;
	UGameInstance(UGameInstance&& _Other) noexcept = delete;
	UGameInstance& operator=(const UGameInstance& _Other) = delete;
	UGameInstance& operator=(UGameInstance&& _Other) noexcept = delete;

protected:

private:

};

