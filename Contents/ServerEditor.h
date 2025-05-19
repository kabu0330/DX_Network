#pragma once
#include <EnginePlatform/EngineClient.h>
#include <EnginePlatform/EngineServer.h>

#include <EngineCore/EngineGUIWindow.h>


// 설명 :
class UServerEditor : public UEngineGUIWindow
{
public:
	UServerEditor();
	~UServerEditor();

	void CreateServer(std::shared_ptr<UEngineServer> _Net);
	void Connect(std::shared_ptr<UEngineClient> _Net);

protected:
	void OnGUI(float _DeltaTime) override;

	void CreateNetObject(std::shared_ptr<UUserAccessPacket> _Packet);

private:
	std::string IP = "127.0.0.1";
	int Port = 20000;
	std::shared_ptr<UUserAccessPacket> UserAccessPacket;

	std::shared_ptr<class UEngineServer> Server;

private:
	// delete Function
	UServerEditor(const UServerEditor& _Other) = delete;
	UServerEditor(UServerEditor&& _Other) noexcept = delete;
	UServerEditor& operator=(const UServerEditor& _Other) = delete;
	UServerEditor& operator=(UServerEditor&& _Other) noexcept = delete;
};

