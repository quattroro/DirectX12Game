#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "InstancingBuffer.h"


MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	_material->PushGraphicsData();

	_mesh->Render();
}
void MeshRenderer::Render(shared_ptr<InstancingBuffer>& buffer)
{
	//기존에는 데이터를 넘겨줄때 params의 b1을 통해서 넣어줬었지만
	//인스턴싱을 사용할때는 VS_IN으로 직접 넣어줄 것이기 때문에 
	//GetTransform()->PushData(); 는 필요가 없다.

	buffer->PushData();
	_material->PushGraphicsData();
	_mesh->Render(buffer);
}

uint64 MeshRenderer::GetInstanceID()
{
	if (_mesh == nullptr || _material == nullptr)
		return 0;
	//각각의 인스턴스 아이디는 해당 규칙으로 만들어 진다.
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}