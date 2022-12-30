#pragma once
#include "Object.h"
class Material;

// [유니티짱]과 같이 정점으로 이루어진 물체
//유니티에서는 해당 클래스의 역할을 meshrenderer에서 한다.
class Mesh:public Object
{
public:
	Mesh();
	virtual ~Mesh();


	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render(uint32 instanceCount = 1);//렌더링을 할때 인스턴스 카운트를 받아온다.
	void Render(shared_ptr<class InstancingBuffer>& buffer);

	//void SetTransform(const Transform& t) { _transform = t; }

	////void SetTexture(shared_ptr<Texture> tex) { _tex = tex; }
	//void SetMaterial(shared_ptr<Material> mat) { _mat = mat; }
private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);



private:
	//정점버퍼
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	//인덱스버퍼
	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW	_indexBufferView = {};
	uint32 _indexCount = 0;

	//이젠 여기서 관리를 하는게 아니고 게임오브젝트에 통합됨
	//Transform _transform = {};

	////shared_ptr<Texture> _tex = {};
	//shared_ptr<Material> _mat = {};
};

