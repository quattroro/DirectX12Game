#pragma once
#include "Texture.h"

//���� ���ٽ�, ����ü�� ��� �ؽ��ķ� ��ü �� �� �ִ�.
//���� ü�� ������ RTV�� ���� ������ ���⿡�� �ؽ��ķ� ��ü�Ͽ� ����Ѵ�.
//�� �׷��� ����.cpp���� ���� �����Ѵ�.
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
	//���������� �׸� ���
	shared_ptr<Texture> target;
	//�ڽ��� ����
	float clearColor[4];
};

//�������� ���� Ÿ��(�ؽ���)�� �Ѳ����� �����Ѵ�.
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

	//� �׷����� ��� �� ���ΰ�
	RENDER_TARGET_GROUP_TYPE		_groupType;

	//���� Ÿ�ٵ�
	vector<RenderTarget>			_rtVec;
	uint32							_rtCount;

	//���� ���ٽ� ���� �ؽ���
	shared_ptr<Texture>				_dsTexture;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;

private:
	uint32							_rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE		_dsvHeapBegin;

	//���۸� �츮�� �׸��°Ͱ� �������� ����ϴ°��� ���ļ� �������Ÿ��� ������ �߻���
	//�׿����� ����� ���� Ÿ�̹��� �������� �ʿ䰡 ����
private:
	D3D12_RESOURCE_BARRIER			_targetToResource[8];
	D3D12_RESOURCE_BARRIER			_resourceToTarget[8];
};

