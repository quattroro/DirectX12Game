#pragma once

//[계약서 / 결재]

//
//루트 시그니쳐를 사용하도록 CommandQueue 의 RenderBegin 에서 해준다.
class RootSignature
{
public:

	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }
private:
	ComPtr<ID3D12RootSignature> _signature;



};

