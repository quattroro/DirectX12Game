#pragma once


// ************************
// GraphicsDescriptorHeap
// ************************

//테이블 자체에 인자를 넘겨줄때 사용
//루트 시그니쳐를 만들어 줄때 만들어주는 데스크립터 테이블을 편하게 뭔하는 값을 세팅 할 수 있도록 해준다.
class GraphicsDescriptorHeap
{
public:
	void Init(uint32 count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:

	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64					_handleSize = 0;
	uint64					_groupSize = 0;
	uint64					_groupCount = 0;

	uint32					_currentGroupIndex = 0;
};



// ************************
// ComputeDescriptorHeap
// ************************

class ComputeDescriptorHeap
{
public:
	void Init();

	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	void SetUAV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, UAV_REGISTER reg);

	void CommitTable();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UAV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:

	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64						_handleSize = 0;
};
