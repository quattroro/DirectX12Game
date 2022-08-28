#pragma once
//�ؽ��ĸ� �ε��ϰ� �����ϴ� Ŭ����
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

	//�� ģ���� �並 �ϳ��� ��� �Ѵ�.(�ϳ��� ó���� �ε� �ϰ� ��� ����� ���̱� ������)
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle;
};

