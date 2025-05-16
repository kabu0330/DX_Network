#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// 설명 : 리소스 관리
class UEngineResourceManager : public UObject
{
public:
	UEngineResourceManager() {}
	~UEngineResourceManager()	{}

	static std::string ToUpperName(std::string_view _Name)
	{
		return UEngineString::ToUpper(_Name);
	}

	template<typename ResType>
	ENGINEAPI static std::shared_ptr<ResType> Find(std::string_view _Name)
	{
		const type_info& Info = typeid(ResType);
		return std::dynamic_pointer_cast<ResType>(Find(Info.name(), _Name.data())); // 부모타입 포인터를 자식타입(ResType)으로 변환
		// 실패시 std::shared_ptr을 nullptr로 반환
	}
	ENGINEAPI static std::shared_ptr<UEngineResourceManager> Find(std::string_view _ResName, std::string_view _Name);

	// 리소스 관리 함수들은 코어에서만 사용
	template<typename ResType>
	ENGINEAPI static void AddResource(std::shared_ptr<UEngineResourceManager> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		AddResource(_Res, Info.name(), _Name, _Path);
	}
	ENGINEAPI static void AddResource(std::shared_ptr<UEngineResourceManager> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);

	template<typename ResType>
	ENGINEAPI static void AddResourceThreadSafe(std::shared_ptr<UEngineResourceManager> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		AddResourceThreadSafe(_Res, Info.name(), _Name, _Path);
	}
	ENGINEAPI static void AddResourceThreadSafe(std::shared_ptr<UEngineResourceManager> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);


	ENGINEAPI UEnginePath GetPath()
	{
		return ResourcePath;
	}

	static bool Contains(std::string_view _Name)
	{
		return RegisterResources.contains(_Name.data());
	}

	static void Release()
	{
		RegisterResources.clear();
	}

protected:
	UEnginePath ResourcePath;

private:
	//                               텍스처                aaaa.png       데이터
	//                               버텍스버퍼             Box             데이터
	ENGINEAPI static inline std::map<std::string, std::map<std::string, std::shared_ptr<UEngineResourceManager>>> RegisterResources;


private:
	// delete Function
	UEngineResourceManager(const UEngineResourceManager& _Other) = delete;
	UEngineResourceManager(UEngineResourceManager&& _Other) noexcept = delete;
	UEngineResourceManager& operator=(const UEngineResourceManager& _Other) = delete;
	UEngineResourceManager& operator=(UEngineResourceManager&& _Other) noexcept = delete;
};


