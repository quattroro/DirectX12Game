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
	BYTE* _mappedBuffer = nullptr;//데이터를 복사할때 사용할 map이 되어진 버퍼영역
	uint32					_elementSize = 0;//각 요소의 크기
	uint32					_elementCount = 0;//각 요소의 개수

	uint32					_currentIndex = 0;//현재 사용된 인덱스 

	//컨스턴트 버퍼 힙, 데스크립터 힙

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;//성능때문에 사용할 모든 힙을 한번에 생성하고 사용 할 것이기 때문에 사용할 위치를 알기 위해

};

