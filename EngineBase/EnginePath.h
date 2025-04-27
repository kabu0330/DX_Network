#pragma once
#include <filesystem>

// 설명 :
class UEnginePath
{
public:
	ENGINEAPI UEnginePath();
	ENGINEAPI UEnginePath(std::string_view _Path);
	ENGINEAPI UEnginePath(std::filesystem::path _Path);
	ENGINEAPI virtual ~UEnginePath();

	ENGINEAPI bool IsExists();
	ENGINEAPI void MoveParent();

	ENGINEAPI std::string GetPathToString();

	// 파일명 + 확장자 포함
	ENGINEAPI std::string GetFileName();

	// 파일명 + 확장자 포함
	ENGINEAPI std::string GetDirectoryName();

	// 확장자
	ENGINEAPI std::string GetExtension();

	ENGINEAPI bool MoveParentToDirectory(std::string_view _Path);

	ENGINEAPI bool Move(std::string_view _Path);

	ENGINEAPI bool IsDirectory();

	ENGINEAPI bool IsFile();

	ENGINEAPI void Append(std::string_view _AppendName);

protected:
	std::filesystem::path Path;

private:
};

