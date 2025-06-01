#pragma once
#include <map>
#include <EngineBase/EngineDefine.h>
#include <memory>

// Ό³Έν :
class UNetObject : public std::enable_shared_from_this<UNetObject>
{
public:
	ENGINEAPI UNetObject();
	ENGINEAPI virtual ~UNetObject();

	ENGINEAPI void InitNetObject(int _ObjectToken, int _SessionToken);

	ENGINEAPI void InitActorData(AActor* _Actor);

	ENGINEAPI static bool		 IsNetObject (int _Token);
	ENGINEAPI static UNetObject* GetNetObject(int _Token);

	template<typename Type>
	static Type* GetConvertNetObject(int _Token)
	{
		return dynamic_cast<Type*>(GetNetObject(_Token));
	}

	template<typename PacketType>
	std::shared_ptr<PacketType> CreatePacket()
	{
		std::shared_ptr<PacketType> NewPacket = std::make_shared<PacketType>();
		NewPacket->SetSessionToken(SessionToken);
		NewPacket->SetObjectToken(ObjectToken);
		return NewPacket;
	}

	void SetControllOff()
	{
		bIsControll = false;
	}

	bool IsControll() const
	{
		return bIsControll;
	}

	int GetObjectToken() const
	{
		return ObjectToken;
	}

	std::shared_ptr<class UNetHandler> GetNetHandler()
	{
		return NetHandler;
	}

protected:

private:
	bool bIsControll = true;
	int SessionToken = -1;
	int ObjectToken = -1;

	inline static std::map<int, UNetObject*> AllNetObjects;
	std::shared_ptr<class UNetHandler> NetHandler = nullptr;

private:
	// delete Function
	UNetObject(const UNetObject& _Other) = delete;
	UNetObject(UNetObject&& _Other) noexcept = delete;
	UNetObject& operator=(const UNetObject& _Other) = delete;
	UNetObject& operator=(UNetObject&& _Other) noexcept = delete;
};

