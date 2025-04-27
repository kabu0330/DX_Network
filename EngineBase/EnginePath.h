#pragma once
#include <filesystem>

// ���� :
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

	// ���ϸ� + Ȯ���� ����
	ENGINEAPI std::string GetFileName();

	// ���ϸ� + Ȯ���� ����
	ENGINEAPI std::string GetDirectoryName();

	// Ȯ����
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

