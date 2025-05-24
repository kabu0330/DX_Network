#pragma once
#include <string>
#include <vector>

// Ό³Έν :
class UChatManager
{
public:
	UChatManager() = default;

	void AddChatMessage(const std::string& _Chat)
	{
		ChatMessages.insert(ChatMessages.begin(), _Chat);

		if (static_cast<int>(ChatMessages.size()) > MaxLines)
		{
			ChatMessages.pop_back();
		}
	}

	const std::vector<std::string>& GetChatMessages()
	{
		return ChatMessages;
	}

protected:

private:
	std::vector<std::string> ChatMessages;
	int MaxLines = 10;

	// delete Function
	UChatManager(const UChatManager& _Other) = delete;
	UChatManager(UChatManager&& _Other) noexcept = delete;
	UChatManager& operator=(const UChatManager& _Other) = delete;
	UChatManager& operator=(UChatManager&& _Other) noexcept = delete;
};

