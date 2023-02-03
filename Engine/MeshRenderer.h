#pragma once
#include "Component.h"

class Mesh;
class Material;

// [32][32]
union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;
};


class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	shared_ptr<Material> GetMaterial() { return _material; }

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	//�ν��Ͻ� ������ ���
	void Render();
	//�ν��Ͻ� ������ ���
	void Render(shared_ptr<class InstancingBuffer>& buffer);
	//�׸��ڸ� �׷��ִ� ����
	void RenderShadow();

	uint64 GetInstanceID();

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};

