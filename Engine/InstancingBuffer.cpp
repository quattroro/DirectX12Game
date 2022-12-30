#include "pch.h"
#include "InstancingBuffer.h"
#include "Engine.h"

InstancingBuffer::InstancingBuffer()
{
}

InstancingBuffer::~InstancingBuffer()
{
}

//개수를 넣어주면 해당 개수만큼 인스턴싱 버퍼를 만들어 준다.
void InstancingBuffer::Init(uint32 maxCount)
{
	_maxCount = maxCount;

	const int32 bufferSize = sizeof(InstancingParams) * maxCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_buffer));
}

//매 프레임마다 호출되어 데이터를 초기화 해준다.
void InstancingBuffer::Clear()
{
	_data.clear();
}

//데이터를 추가해주는 부분
void InstancingBuffer::AddData(InstancingParams& params)
{
	_data.push_back(params);
}

//렌더링을 할때 해당 함수가 호출된다.
void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount)
		Init(dataCount);

	const uint32 bufferSize = dataCount * sizeof(InstancingParams);

	//사용할 데이터들을 복사해준다.
	void* dataBuffer = nullptr;
	D3D12_RANGE readRange{ 0, 0 };
	_buffer->Map(0, &readRange, &dataBuffer);
	memcpy(dataBuffer, &_data[0], bufferSize);
	_buffer->Unmap(0, nullptr);

	_bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
	_bufferView.StrideInBytes = sizeof(InstancingParams);
	_bufferView.SizeInBytes = bufferSize;
}