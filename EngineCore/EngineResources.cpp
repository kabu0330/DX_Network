#include "PreCompile.h"
#include "EngineResources.h"
#include "EngineTexture.h"
#include <mutex>

std::mutex ResCritical_Section;

std::shared_ptr<UEngineResources> UEngineResources::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == ResourceMap[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return ResourceMap[_ResName.data()][UpperString];
}

//                                              �޸� ���                    �ڷ���                     ���� ���� �̸�             ���(��������)
void UEngineResources::PushResource(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	//       �ؽ�ó��                  Player.png�� ����ִ��� Ȯ��.
	if (true == ResourceMap[_Info.data()].contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���ҽ��Դϴ�. " + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	_Res->SetName(UpperName); // �ؽ�ó �̸�
	_Res->Path = _Path; // �ؽ�ó ���
	ResourceMap[_Info.data()].insert({UpperName, _Res }); // �ڷ����� �������� �ٽ�, �ؽ�ó �̸��� �������� �ؽ�ó �����͸� ����

	return;
}

void UEngineResources::PushResourceThreadSafe(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	_Res->SetName(UpperName);
	_Res->Path = _Path;

	{
		std::lock_guard<std::mutex> LockGuard(ResCritical_Section);
		if (true == ResourceMap[_Info.data()].contains(UpperName))
		{
			MSGASSERT("�̹� �ε��� ���ҽ��Դϴ�. " + std::string(_Info.data()) + " " + _Name.data());
			return;
		}

		ResourceMap[_Info.data()].insert({ UpperName, _Res });
	}
}
