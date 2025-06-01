#pragma once
#include <string>

// Ό³Έν :
class UContentResource
{
public:
	UContentResource();
	virtual ~UContentResource() = 0;

	static void LoadContentsResource(std::string_view _Path);
	static void LoadResourceDirectory();
	static void LoadFolder();
	static void LoadSprite();
	static void LoadShaderResource();
protected:

private:
	static void LoadResource();
	static void LoadFont();

private:
	// delete Function
	UContentResource(const UContentResource& _Other) = delete;
	UContentResource(UContentResource&& _Other) noexcept = delete;
	UContentResource& operator=(const UContentResource& _Other) = delete;
	UContentResource& operator=(UContentResource&& _Other) noexcept = delete;
};

