#pragma once

// Ό³Έν :
class EngineNetwork
{
public:
	EngineNetwork();
	~EngineNetwork();



protected:

private:
	// delete Function
	EngineNetwork(const EngineNetwork& _Other) = delete;
	EngineNetwork(EngineNetwork&& _Other) noexcept = delete;
	EngineNetwork& operator=(const EngineNetwork& _Other) = delete;
	EngineNetwork& operator=(EngineNetwork&& _Other) noexcept = delete;
};

