#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;//데이터를 복사할때 사용할 map이 되어진 버퍼영역
	uint32					_elementSize = 0;//각 요소의 크기
	uint32					_elementCount = 0;//각 요소의 개수

	uint32					_currentIndex = 0;//현재 사용된 인덱스 
};

