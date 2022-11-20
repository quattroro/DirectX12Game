#pragma once

class SwapChain;
class DescriptorHeap;

// ************************
// GraphicsCommandQueue
// ************************

//
//기존에는 device 에서 한번에 처리하던 기능이 따로 분화됨
//일감을 넘겨준다.
class GraphicsCommandQueue
{
public:
	~GraphicsCommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCmdList() { return _cmdList; }
	ComPtr<ID3D12GraphicsCommandList>	GetResourceCmdList() { return _resCmdList; }
private:

	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			_cmdQueue;

	//CommandQueue에 추가된 명령들은 이 할당자의 메모리에 저장된다.
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;

	//CommandQueue의 명령 리스트
	//한 할당자를 여러 명령 리스트에 연관 시켜도 되지만
	//명령들을 리스트에 동시에 기록할 수는 없다.
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	//텍스쳐 정보를 넘겨주기 위한 명령 리스트를 따로 만들어 준다.
	ComPtr<ID3D12CommandAllocator> _resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> _resCmdList;

	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};

// ************************
// ComputeCommandQueue
// ************************

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();
	void FlushComputeCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetComputeCmdList() { return _cmdList; }

private:
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;
};