#pragma once
#include "EngineResources.h"
#include "EngineDeviceBuffer.h"
#include "EngineEnums.h"

// ���� :
class UEngineConstantBuffer : public UEngineResources, public UEngineDeviceBuffer
{
public:
	// constrcuter destructer
	UEngineConstantBuffer();
	~UEngineConstantBuffer();

	// delete Function
	UEngineConstantBuffer(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer(UEngineConstantBuffer&& _Other) noexcept = delete;
	UEngineConstantBuffer& operator=(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer& operator=(UEngineConstantBuffer&& _Other) noexcept = delete;

	// ������۸� ����ų� ã�ų�
	static std::shared_ptr<UEngineConstantBuffer> CreateOrFind(UINT _Byte, const std::string_view& _Name);

	void ChangeData(void* _Data, UINT _Size);
	void Setting(EShaderType _Type, UINT _BindIndex);

	static void Release();

protected:

private:
	//           ����Ʈ           ����� ���� �̸�                     �������
	// � �̸����� ������۸� �� ����Ʈ �����Ұų�?
	static std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> BufferMap;

	void ResCreate(UINT _Byte);
};

