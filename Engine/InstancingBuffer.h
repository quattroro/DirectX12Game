#pragma once

//해당 물체의 정보들을 구조체로 묶어서 관리하고 보내준다.
struct InstancingParams
{
	Matrix matWorld;//World
	Matrix matWV;//World, View
	Matrix matWVP;//World, View, Projection
};


class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

	void Init(uint32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	uint32						GetCount() { return static_cast<uint32>(_data.size()); }
	ComPtr<ID3D12Resource>		GetBuffer() { return _buffer; }
	D3D12_VERTEX_BUFFER_VIEW	GetBufferView() { return _bufferView; }

	void	SetID(uint64 instanceId) { _instanceId = instanceId; }
	uint64	GetID() { return _instanceId; }

private:
	uint64						_instanceId = 0;
	ComPtr<ID3D12Resource>		_buffer;
	D3D12_VERTEX_BUFFER_VIEW	_bufferView;

	uint32						_maxCount = 0;

	vector<InstancingParams>	_data;//넘겨줄 구조체들을 벡터로 관리하고 있는다.
	//이렇게 가지고 있다가 PushData가 실행되면 
	//uint64						_instanceId = 0;
	//ComPtr<ID3D12Resource>		_buffer;
	//D3D12_VERTEX_BUFFER_VIEW	_bufferView;
	//이쪽으로 데이터를 옮겨주면서 사용하게 된다.
};

