#include "pch.h"
#include "RootSignature.h"

//���⼱ �ϴ� �ʱ�ȭ�� ���ְ� ������ ����� Mesh�� Render���� ���ش�.
void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV/*�뵵*/,CBV_REGISTER_COUNT/*�*/, 0/*���°���� ����� ���ΰ�*/),
	};

	CD3DX12_ROOT_PARAMETER param[1];
	//param[0].InitAsConstantBufferView(0);//0�� -> b0 -> CBV
	//param[1].InitAsConstantBufferView(1);//1�� -> b1 -> CBV

	param[0].InitAsDescriptorTable(_countof(ranges), ranges);


	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param);

	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}