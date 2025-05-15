#include "PreCompile.h"
#include "Mesh.h"

UMesh::UMesh()
{
}

UMesh::~UMesh()
{
}

std::shared_ptr<UMesh> UMesh::CreateWithBuffers(std::string_view _Name, std::string_view _VertexBuffer, std::string_view _IndexBuffer)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[Mesh Create Failed] \n 중복된 메시 이름 : " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UMesh> NewMesh = std::make_shared<UMesh>();
	PushResource<UMesh>(NewMesh, _Name, "");
	NewMesh->VertexBuffer = UEngineVertexBuffer::Find<UEngineVertexBuffer>(_VertexBuffer);
	NewMesh->IndexBuffer = UEngineIndexBuffer::Find<UEngineIndexBuffer>(_IndexBuffer);

	return NewMesh;
}
