#pragma once
#include "Texture.h"

//뎁스 스텐실, 스왑체인 모두 텍스쳐로 대체 할 수 있다.
//스왑 체인 에서는 RTV에 관한 모든것을 여기에서 텍스쳐로 대체하여 사용한다.
//이 그룹은 엔진.cpp에서 만들어서 관리한다.
enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	G_BUFFER, // POSITION, NORMAL, COLOR

	LIGHTING, // DIFFUSE LIGHT, SPECULAR LIGHT
	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 2,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END)
};

struct RenderTarget
{
	//실질적으로 그릴 대상
	shared_ptr<Texture> target;
	//자신의 색상
	float clearColor[4];
};

//여러개의 렌더 타겟(텍스쳐)을 한꺼번에 관리한다.
class RenderTargetGroup
{
public:
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);

	void OMSetRenderTargets(uint32 count, uint32 offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(uint32 index);
	void ClearRenderTargetView();

	shared_ptr<Texture> GetRTTexture(uint32 index) { return _rtVec[index].target; }
	shared_ptr<Texture> GetDSTexture() { return _dsTexture; }

	void WaitTargetToResource();
	void WaitResourceToTarget();

private:

	//어떤 그룹으로 사용 할 것인가
	RENDER_TARGET_GROUP_TYPE		_groupType;

	//렌더 타겟들
	vector<RenderTarget>			_rtVec;
	uint32							_rtCount;

	//깊이 스텐실 버퍼 텍스쳐
	shared_ptr<Texture>				_dsTexture;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;

private:
	uint32							_rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE		_dsvHeapBegin;

	//버퍼를 우리가 그리는것과 엔진에서 사용하는것이 겹쳐서 지지직거리를 현상이 발생함
	//그에따라서 베리어를 만들어서 타이밍을 조절해줄 필요가 생김
private:
	D3D12_RESOURCE_BARRIER			_targetToResource[8];
	D3D12_RESOURCE_BARRIER			_resourceToTarget[8];
};

