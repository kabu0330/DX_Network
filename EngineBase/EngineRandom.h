#pragma once
#include <random>

// º≥∏Ì :
class UEngineRandom
{
public:
	// constrcuter destructer
	UEngineRandom() {}
	~UEngineRandom() {}

	// delete Function
	UEngineRandom(const UEngineRandom& _Other) = delete;
	UEngineRandom(UEngineRandom&& _Other) noexcept = delete;
	UEngineRandom& operator=(const UEngineRandom& _Other) = delete;
	UEngineRandom& operator=(UEngineRandom&& _Other) noexcept = delete;

	void SetSeed(__int64 _Seed)
	{
		MtGen = std::mt19937_64(_Seed);
	}

	int RandomInt(int _Min, int _Max)
	{
		if (_Max < _Min)
		{
			int SwapValue = _Min;
			_Min = _Max;
			_Max = SwapValue;
		}

		// _Min ~ _Max ∑£¥˝∞™ ªÃæ∆¡‡
		std::uniform_int_distribution<int> RandomCreate(_Min, _Max);

		// MtGen ¡¶≥◊∑π¿Ã≈Õ Ω·º≠
		// std::mt19937_64 ∏ﬁ∏£«Ó ∆Æ¿ßΩ∫≈Õ æÀ∞Ì∏Æ¡Ú Ω·º≠ ∏∏µÈæÓ¡‡.
		return RandomCreate.operator()(MtGen);
	}

	float Randomfloat(float _Min, float _Max)
	{
		if (_Max < _Min)
		{
			float SwapValue = _Min;
			_Min = _Max;
			_Max = SwapValue;
		}

		// _Min ~ _Max ∑£¥˝∞™ ªÃæ∆¡‡
		std::uniform_real_distribution<float> RandomCreate(_Min, _Max);
		return RandomCreate.operator()(MtGen);
	}

protected:

private:

	std::mt19937_64 MtGen = std::mt19937_64(time(nullptr));

	//std::well512
};
