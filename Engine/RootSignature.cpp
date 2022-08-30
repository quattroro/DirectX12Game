#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

//여기선 일단 초기화를 해주고 실제로 사용은 Mesh의 Render에서 해준다.
void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();
}


void RootSignature::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV/*용도*/,CBV_REGISTER_COUNT/*몇개*/, 0/*몇번째부터 사용할 것인가*/),//b0~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV/*용도*/,SRV_REGISTER_COUNT/*몇개*/, 0/*몇번째부터 사용할 것인가*/),//t0~t4
	};

	CD3DX12_ROOT_PARAMETER param[1];
	//param[0].InitAsConstantBufferView(0);//0번 -> b0 -> CBV
	//param[1].InitAsConstantBufferView(1);//1번 -> b1 -> CBV

	param[0].InitAsDescriptorTable(_countof(ranges), ranges);


	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);
	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param);
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

//기본 형태로 만들어 준다.
void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}