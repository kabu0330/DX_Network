#pragma once
#include "EngineResources.h"
#include "EngineDeviceBuffer.h"
#include "EngineEnums.h"

// 설명 :
class UEngineConstantBuffer : public UEngineResources, public UEngineDeviceBuffer
{
public:
	UEngineConstantBuffer();
	~UEngineConstantBuffer();

	// 상수버퍼를 만들거나 찾거나
	static std::shared_ptr<UEngineConstantBuffer> CreateOrFind(UINT _Byte, const std::string_view& _Name);

	void ChangeData(void* _Data, UINT _Size);
	void Setting(EShaderType _Type, UINT _BindIndex);

	static void Release();

protected:

private:
	//           바이트           사용자 정의 이름                     상수버퍼
	// 어떤 이름으로 상수버퍼를 몇 바이트 저장할거냐?
	static std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> BufferMap;

	void CreateViewObject(UINT _Byte);


private:
	// delete Function
	UEngineConstantBuffer(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer(UEngineConstantBuffer&& _Other) noexcept = delete;
	UEngineConstantBuffer& operator=(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer& operator=(UEngineConstantBuffer&& _Other) noexcept = delete;
};

