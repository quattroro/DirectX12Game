#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	//D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

	//desc heap 
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();

	//desc heap
	void CreateView();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;//�����͸� �����Ҷ� ����� map�� �Ǿ��� ���ۿ���
	uint32					_elementSize = 0;//�� ����� ũ��
	uint32					_elementCount = 0;//�� ����� ����

	uint32					_currentIndex = 0;//���� ���� �ε��� 

	//������Ʈ ���� ��, ����ũ���� ��

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;//���ɶ����� ����� ��� ���� �ѹ��� �����ϰ� ��� �� ���̱� ������ ����� ��ġ�� �˱� ����

};

