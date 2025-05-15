#pragma once
#include <functional>

// Ό³Έν :
class UEngineDelegate
{
public:
	ENGINEAPI UEngineDelegate();
	ENGINEAPI UEngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	ENGINEAPI ~UEngineDelegate();

	bool IsBind()
	{
		return false == Functions.empty();
	}

	void operator+=(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	void operator()()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()>& Function = *StartIter;
			Function();
		}
	}

	void Clear()
	{
		Functions.clear();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};

