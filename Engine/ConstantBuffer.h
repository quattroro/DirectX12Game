#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void Clear();
	void PushGraphicsData(void* buffer, uint32 size);
	void SetGraphicsGlobalData(void* buffer, uint32 size);//라이트에 사용하는 정보는 따로 사용


	void PushComputeData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;//데이터를 복사할때 사용할 map이 되어진 버퍼영역
	uint32					_elementSize = 0;//각 요소의 크기
	uint32					_elementCount = 0;//각 요소의 개수

//컨스턴트 버퍼 힙, 데스크립터 힙
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;//성능때문에 사용할 모든 힙을 한번에 생성하고 사용 할 것이기 때문에 사용할 위치를 알기 위해

	uint32					_currentIndex = 0;//현재 사용된 인덱스 

	CBV_REGISTER			_reg = {};
};

