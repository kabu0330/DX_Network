#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// ���� : ���ҽ� ����
class UEngineResources : public UObject
{
public:
	UEngineResources() {}
	~UEngineResources()	{}

	static std::string ToUpperName(std::string_view _Name)
	{
		return UEngineString::ToUpper(_Name);
	}

	template<typename ResType>
	ENGINEAPI static std::shared_ptr<ResType> Find(std::string_view _Name)
	{
		const type_info& Info = typeid(ResType);
		return std::dynamic_pointer_cast<ResType>(Find(Info.name(), _Name.data())); // �θ�Ÿ�� �����͸� �ڽ�Ÿ��(ResType)���� ��ȯ
		// ���н� std::shared_ptr�� nullptr�� ��ȯ
	}
	ENGINEAPI static std::shared_ptr<UEngineResources> Find(std::string_view _ResName, std::string_view _Name);

	// ���ҽ� ���� �Լ����� �ھ���� ���
	template<typename ResType>
	ENGINEAPI static void PushResource(std::shared_ptr<UEngineResources> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushResource(_Res, Info.name(), _Name, _Path);
	}
	ENGINEAPI static void PushResource(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);

	template<typename ResType>
	ENGINEAPI static void PushResourceThreadSafe(std::shared_ptr<UEngineResources> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushResourceThreadSafe(_Res, Info.name(), _Name, _Path);
	}
	ENGINEAPI static void PushResourceThreadSafe(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);


	ENGINEAPI UEnginePath GetPath()
	{
		return Path;
	}

	static bool Contains(std::string_view _Name)
	{
		return ResourceMap.contains(_Name.data());
	}

	static void Release()
	{
		ResourceMap.clear();
	}

protected:
	UEnginePath Path;

private:
	//                               �ؽ�ó                aaaa.png       ������
	//                               ���ؽ�����             Box             ������
	ENGINEAPI static inline std::map<std::string, std::map<std::string, std::shared_ptr<UEngineResources>>> ResourceMap;


private:
	// delete Function
	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;
};


