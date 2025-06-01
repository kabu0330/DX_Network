#pragma once
#include <EnginePlatform/EngineClient.h>
#include <EnginePlatform/EngineServer.h>
#include <EnginePlatform/EngineIOCPServer.h>

#include <EngineCore/EngineGUIWindow.h>


// 설명 :
class UTetrisPlayEditor : public UEngineGUIWindow
{
public:
	UTetrisPlayEditor();
	~UTetrisPlayEditor();


	void CreateServer(std::shared_ptr<UEngineServer> _Net);
	void Connect(std::shared_ptr<UEngineClient> _Net);

protected:
	void OnGUI(float _DeltaTime) override;


private:
	std::string IP = "127.0.0.1";
	int Port = 20000;
	std::shared_ptr<UUserAccessPacket> UserAccessPacket = nullptr;

	std::shared_ptr<class UEngineServer> Server = nullptr;
	std::shared_ptr<class UEngineIOCPServer> IOCPServer = nullptr;

private:
	// delete Function
	UTetrisPlayEditor(const UTetrisPlayEditor& _Other) = delete;
	UTetrisPlayEditor(UTetrisPlayEditor&& _Other) noexcept = delete;
	UTetrisPlayEditor& operator=(const UTetrisPlayEditor& _Other) = delete;
	UTetrisPlayEditor& operator=(UTetrisPlayEditor&& _Other) noexcept = delete;
};

