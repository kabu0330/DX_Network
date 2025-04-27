#pragma once
#include <EngineCore/EngineGUIWindow.h>

// ���� :
class UContentsEditorGUI : public UEngineGUIWindow
{
public:
	// constrcuter destructer
	UContentsEditorGUI();
	~UContentsEditorGUI();

	// delete Function
	UContentsEditorGUI(const UContentsEditorGUI& _Other) = delete;
	UContentsEditorGUI(UContentsEditorGUI&& _Other) noexcept = delete;
	UContentsEditorGUI& operator=(const UContentsEditorGUI& _Other) = delete;
	UContentsEditorGUI& operator=(UContentsEditorGUI&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

private:

};

