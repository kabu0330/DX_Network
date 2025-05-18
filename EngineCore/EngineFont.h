#pragma once
#include "EngineResourceManager.h"

#include "ThirdParty/FW1FontWrapper/Inc/FW1FontWrapper.h"

// Ό³Έν :
class UEngineFont : public UEngineResourceManager
{
public:
	UEngineFont();
	~UEngineFont();

	ENGINEAPI static std::shared_ptr<UEngineFont> LoadFont(std::string_view _Name, std::string_view _Path);

	void FontDraw(const std::string& _Text, float _FontScale, const FVector& _Pos, const UColor& _Color = UColor::BLACK, FW1_TEXT_FLAG Flag = FW1_LEFT);

protected:

private:
	Microsoft::WRL::ComPtr<IFW1FontWrapper> Font;
	void LoadResource(std::string_view _Path);

private:
	// delete Function
	UEngineFont(const UEngineFont& _Other) = delete;
	UEngineFont(UEngineFont&& _Other) noexcept = delete;
	UEngineFont& operator=(const UEngineFont& _Other) = delete;
	UEngineFont& operator=(UEngineFont&& _Other) noexcept = delete;
};

