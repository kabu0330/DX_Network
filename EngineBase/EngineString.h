#pragma once
#include <string>

// 설명 : std::string 기능 래퍼
class UEngineString
{
public:
	~UEngineString();

	ENGINEAPI static std::string ToUpper(std::string_view _string);

	ENGINEAPI static std::string InterString(const std::string& _Text, std::string_view _Start, std::string_view _End, size_t& _Offset);

	ENGINEAPI static std::wstring AnsiToUnicode(std::string_view _Name);

	ENGINEAPI static std::string UniCodeToUTF8(std::wstring_view _Name);

	ENGINEAPI static std::string AnsiToUTF8(std::string_view _Name);

private:
	UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;
};

