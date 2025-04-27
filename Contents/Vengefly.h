#pragma once
#include "Monster.h"

// Ό³Έν :
class AVengefly : public AMonster
{
public:
	// constrcuter destructer
	AVengefly();
	~AVengefly();

	// delete Function
	AVengefly(const AVengefly& _Other) = delete;
	AVengefly(AVengefly&& _Other) noexcept = delete;
	AVengefly& operator=(const AVengefly& _Other) = delete;
	AVengefly& operator=(AVengefly&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:

};

