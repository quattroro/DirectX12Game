#pragma once

//[계약서 / 결재]

//
//루트 시그니쳐를 사용하도록 CommandQueue 의 RenderBegin 에서 해준다. 
class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetGraphicsRootSignature() { return _graphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeRootSignature() { return _computeRootSignature; }

private:
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();

private:
	D3D12_STATIC_SAMPLER_DESC	_samplerDesc; //컴퓨트에서는 샘플러 데스크립션이 필요 없다.
	ComPtr<ID3D12RootSignature>	_graphicsRootSignature;
	ComPtr<ID3D12RootSignature>	_computeRootSignature;
};


