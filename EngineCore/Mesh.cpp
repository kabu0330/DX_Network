#include "PreCompile.h"
#include "Mesh.h"

UMesh::UMesh()
{
}

UMesh::~UMesh()
{
}

std::shared_ptr<UMesh> UMesh::Create(std::string_view _Name, std::string_view _VertexBuffer, std::string_view _IndexBuffer)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("[이름 중복] \n 메시 이름을 변경해주세요." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UMesh> NewMesh = std::make_shared<UMesh>();
	PushResource<UMesh>(NewMesh, _Name, "");
	NewMesh->VertexBuffer = UEngineVertexBuffer::Find<UEngineVertexBuffer>(_VertexBuffer);
	NewMesh->IndexBuffer = UEngineIndexBuffer::Find<UEngineIndexBuffer>(_IndexBuffer);

	return NewMesh;
}
