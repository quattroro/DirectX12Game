#pragma once

//[��༭ / ����]

//
//��Ʈ �ñ״��ĸ� ����ϵ��� CommandQueue �� RenderBegin ���� ���ش�. 
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
	D3D12_STATIC_SAMPLER_DESC	_samplerDesc; //��ǻƮ������ ���÷� ����ũ������ �ʿ� ����.
	ComPtr<ID3D12RootSignature>	_graphicsRootSignature;
	ComPtr<ID3D12RootSignature>	_computeRootSignature;
};


