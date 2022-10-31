#pragma once
#include "Object.h"
//텍스쳐를 로딩하고 관리하는 클래스
//뎁스 스텐실과 스왑체인에서 관리하고 있던정보들을 텍스텨를 이용해서 관리를 하는것으로 바꿀 것이기 때문에 그에따라 텍스쳐를 내부적으로 이용하기 위한 작업들이 필요하다
//뎁스 스텐실과 스왑체인에서 관리하고 있던정보들을 텍스텨를 이용해서 관리를 하는것으로 바꿀 것이기 때문에 그에따라 텍스쳐를 내부적으로 이용하기 위한 작업들이 필요하다
class Texture:public Object
{
public:
	Texture();
	virtual ~Texture();

	virtual void Load(const wstring& path) override;


public:
	void Create(DXGI_FORMAT format, uint32 width, uint32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor = Vec4());

	//스왑 체인에서 하던 일을 여기서 한다.
	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);


public:
	ComPtr<ID3D12Resource> GetTex2D() { return _tex2D; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return _srvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return _rtvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return _dsvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return _uavHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return _srvHeapBegin; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() { return _uavHeapBegin; }


private:
	ScratchImage			 		_image;
	ComPtr<ID3D12Resource>			_tex2D;

	//이 친구는 뷰를 하나만 사용 한다.(하나를 처음에 로딩 하고 계속 사용할 것이기 때문에)
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;

	//기존 srv하나만 사용하던 것에서 rtv와 dsv도 보관 할 수 있도록 만들어 준다.
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	ComPtr<ID3D12DescriptorHeap>	_uavHeap;



private:
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHeapBegin = {};
	D3D12_CPU_DESCRIPTOR_HANDLE		_uavHeapBegin = {};
};

