#pragma once
//텍스쳐를 로딩하고 관리하는 클래스
class Texture
{
public:
	void Init(const wstring& path);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

public:
	void CreateTexture(const wstring& path);
	void CreateView();

private:
	ScratchImage			 		_image;
	ComPtr<ID3D12Resource>			_tex2D;

	//이 친구는 뷰를 하나만 사용 한다.(하나를 처음에 로딩 하고 계속 사용할 것이기 때문에)
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle;
};

