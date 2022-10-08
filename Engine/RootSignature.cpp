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
	/*라이트가 추가 되면서 .CBV를 넘겨주는 방식을 추가할 필요성이 생김
		  기존에는 테이블을 이용히서 모든 정보를 묶어서 한번에 보냈다면
		  라이팅은 그렇에 매 프레임 바다 보낼 필요 없이 한번만 보내면 되기 때문에 b0번을 전역으로 빼서 라이트 용으로 사용한다.
		  이곳을 수정함으로써 TableDiscriptorHeap도 수정할 필요가 생김*/
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV/*용도*/,CBV_REGISTER_COUNT - 1/*몇개*/, 1/*몇번째부터 사용할 것인가*/),//b1~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	};

	CD3DX12_ROOT_PARAMETER param[2];
	//param[0].InitAsConstantBufferView(0);//0번 -> b0 -> CBV
		//param[1].InitAsConstantBufferView(1);//1번 -> b1 -> CBV
	param[0].InitAsConstantBufferView(static_cast<uint32>(CBV_REGISTER::b0)); // b0
	param[1].InitAsDescriptorTable(_countof(ranges), ranges);

	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);
	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param);

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}