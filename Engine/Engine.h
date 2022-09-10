#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"	
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"



class Engine
{
public:
	void Init(const WindowInfo& info);
	void Update();
//public:
//	void LateUpdate();

public:
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	//shared_ptr<ConstantBuffer> GetCB() { return _cb; }
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }

	/*shared_ptr<Input> GetInput() { return _input; }
	shared_ptr<Timer> GetTimer() { return _timer; }*/

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);//������ ũ�⸦ ����

private:
	void ShowFPS();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);

private:
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;//�ϰ��� ��û�Ҷ� �ش� Ŭ������ �̿��ؼ� �ϰ��� ��Ƽ� �ѹ��� ������� ��������ִ� ������ �Ѵ�.
	shared_ptr<SwapChain> _swapChain;//�������
	shared_ptr<RootSignature> _rootSignature;
	//shared_ptr<ConstantBuffer> _cb; //���׸����� �߰� �ϸ鼭 �ϳ��� ����� ���� �ƴϰ� �Ǿ��� ������ vector�ڤ��ᱸ���� �̿��ؼ� �����Ѵ�.
	shared_ptr<TableDescriptorHeap> _tableDescHeap;
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer;

	/*shared_ptr<Input> _input;
	shared_ptr<Timer> _timer;*/

	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
};

