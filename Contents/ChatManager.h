#pragma once
#include <vector>
#include <string>

// Ό³Έν :
class UChatManager
{
public:
	UChatManager() = default;

	void AddMessage(std::string_view _Message)
	{
		Messages.insert(Messages.begin(), _Message.data());

		if (static_cast<int>(Messages.size()) > MaxLines)
		{
			Messages.pop_back();
		}
	}

	const std::vector<std::string>& GetMessages() const
	{
		return Messages;
	}


protected:

private:
	std::vector<std::string> Messages;
	int MaxLines = 10;

	// delete Function
	UChatManager(const UChatManager& _Other) = delete;
	UChatManager(UChatManager&& _Other) noexcept = delete;
	UChatManager& operator=(const UChatManager& _Other) = delete;
	UChatManager& operator=(UChatManager&& _Other) noexcept = delete;
};

