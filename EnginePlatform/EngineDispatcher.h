#pragma once
#include <map>
#include <functional>
#include <memory>

#include <EngineBase/EngineDebug.h>
#include "EngineProtocol.h"

// Ό³Έν :
class EngineDispatcher
{
public:
	EngineDispatcher();
	~EngineDispatcher();

	template<typename PacketType>
	void AddHandler(int _Type, std::function<void(std::shared_ptr<PacketType>)> _Callback)
	{

	}


protected:

private:
	std::map<int, std::function<std::shared_ptr<UEngineProtocol>(UEngineSerializer& _Ser)>> ConvertPacketHandler;
	std::map<int, std::function<void(std::shared_ptr<UEngineProtocol>)>> PacketHandler;


private:
	// delete Function
	EngineDispatcher(const EngineDispatcher& _Other) = delete;
	EngineDispatcher(EngineDispatcher&& _Other) noexcept = delete;
	EngineDispatcher& operator=(const EngineDispatcher& _Other) = delete;
	EngineDispatcher& operator=(EngineDispatcher&& _Other) noexcept = delete;
};

