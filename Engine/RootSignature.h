#pragma once

//[계약서 / 결재]

//
//루트 시그니쳐를 사용하도록 CommandQueue 의 RenderBegin 에서 해준다. 
class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	void CreateSamplerDesc();
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature>	_signature;
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};

