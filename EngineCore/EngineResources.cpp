#include "PreCompile.h"
#include "EngineResources.h"
#include "EngineTexture.h"

std::shared_ptr<UEngineResources> UEngineResources::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == ResMap[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return ResMap[_ResName.data()][UpperString];
}

//                                              메모리 블록                    자료형                     내가 정한 이름             경로(생략가능)
void UEngineResources::PushRes(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	//       텍스처에                  Player.png가 들어있는지 확인.
	if (true == ResMap[_Info.data()].contains(UpperName))
	{
		MSGASSERT("이미 로드한 리소스를 또 로드 하려고 했습니다" + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	// 텍스처
	// 머티리얼

	_Res->SetName(UpperName); // 텍스처 이름
	_Res->Path = _Path; // 텍스처 경로
	ResMap[_Info.data()].insert({UpperName, _Res }); // 자료형을 기준으로 다시, 텍스처 이름을 기준으로 텍스처 데이터를 저장

	return;
}