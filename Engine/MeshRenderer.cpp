#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"


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
	//�������� �����͸� �Ѱ��ٶ� params�� b1�� ���ؼ� �־��������
	//�ν��Ͻ��� ����Ҷ��� VS_IN���� ���� �־��� ���̱� ������ 
	//GetTransform()->PushData(); �� �ʿ䰡 ����.

	buffer->PushData();
	_material->PushGraphicsData();
	_mesh->Render(buffer);
}

void MeshRenderer::RenderShadow()
{
	//�׸��ڴ� ���� �׸��� ���׸����� �����Ѵ�.
	GetTransform()->PushData();
	GET_SINGLE(Resources)->Get<Material>(L"Shadow")->PushGraphicsData();
	_mesh->Render();
}


uint64 MeshRenderer::GetInstanceID()
{
	if (_mesh == nullptr || _material == nullptr)
		return 0;
	//������ �ν��Ͻ� ���̵�� �ش� ��Ģ���� ����� ����.
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}