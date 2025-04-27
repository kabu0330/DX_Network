#pragma once
#include <string>

// Ό³Έν :
class UContentsResource
{
public:
	// constrcuter destructer
	UContentsResource();
	virtual ~UContentsResource() = 0;

	// delete Function
	UContentsResource(const UContentsResource& _Other) = delete;
	UContentsResource(UContentsResource&& _Other) noexcept = delete;
	UContentsResource& operator=(const UContentsResource& _Other) = delete;
	UContentsResource& operator=(UContentsResource&& _Other) noexcept = delete;

	static void LoadContentsResource(std::string_view _Path);
	static void LoadResourceDirectory();
	static void LoadFolder();
	static void LoadSprite();
	static void LoadShaderResource();
protected:

private:
	static void LoadResource();
	static void LoadFont();
};

