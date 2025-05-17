#pragma once

// Ό³Έν :
class UNetObject
{
public:
	UNetObject();
	~UNetObject();



protected:

private:

private:
	// delete Function
	UNetObject(const UNetObject& _Other) = delete;
	UNetObject(UNetObject&& _Other) noexcept = delete;
	UNetObject& operator=(const UNetObject& _Other) = delete;
	UNetObject& operator=(UNetObject&& _Other) noexcept = delete;
};

