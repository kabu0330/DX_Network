#include "PreCompile.h"
#include "EngineResourceManager.h"
#include "EngineTexture.h"
#include <mutex>

std::mutex ResCritical_Section;

std::shared_ptr<UEngineResourceManager> UEngineResourceManager::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == RegisterResources[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return RegisterResources[_ResName.data()][UpperString];
}

//                                              �޸� ���                    �ڷ���                     ���� ���� �̸�             ���(��������)
void UEngineResourceManager::AddResource(std::shared_ptr<UEngineResourceManager> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	//       �ؽ�ó��                  Player.png�� ����ִ��� Ȯ��.
	if (true == RegisterResources[_Info.data()].contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���ҽ��Դϴ�. " + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	_Res->SetName(UpperName); // �ؽ�ó �̸�
	_Res->ResourcePath = _Path; // �ؽ�ó ���
	RegisterResources[_Info.data()].insert({UpperName, _Res }); // �ڷ����� �������� �ٽ�, �ؽ�ó �̸��� �������� �ؽ�ó �����͸� ����

	return;
}

void UEngineResourceManager::AddResourceThreadSafe(std::shared_ptr<UEngineResourceManager> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	_Res->SetName(UpperName);
	_Res->ResourcePath = _Path;

	{
		std::lock_guard<std::mutex> LockGuard(ResCritical_Section);
		if (true == RegisterResources[_Info.data()].contains(UpperName))
		{
			MSGASSERT("�̹� �ε��� ���ҽ��Դϴ�. " + std::string(_Info.data()) + " " + _Name.data());
			return;
		}

		RegisterResources[_Info.data()].insert({ UpperName, _Res });
	}
}
