#pragma once
#include "ServerPawn.h"
#include "ContentEnum.h"

// Ό³Έν :
class ATetromino : public AServerPawn
{
public:
	ATetromino();
	~ATetromino() {};

	void SetType(EMinoType _Type);


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

	void SetControllMode(float _DeltaTime) override;
	


private:
	void CreateRenderers();
	void InitType();
	void ChangeMino();

	std::vector<std::vector<USpriteRenderer*>> MinoRenders;
	EMinoType MinoType = EMinoType::Z_MINO;

private:
	// delete Function
	ATetromino(const ATetromino& _Other) = delete;
	ATetromino(ATetromino&& _Other) noexcept = delete;
	ATetromino& operator=(const ATetromino& _Other) = delete;
	ATetromino& operator=(ATetromino&& _Other) noexcept = delete;
};

