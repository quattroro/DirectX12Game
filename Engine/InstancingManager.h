#pragma once
#include "InstancingBuffer.h"

class GameObject;



//인스턴스 버퍼는 각각의 물체 세트 마다 하나씩 필요하기 때문에 해당 인스턴스들을 관리해주는 역할을 할 매니저 이다.
//이제 앞으로는 카메라에서 물체들을 렌더링 하는것이 아니라 
//인스턴스 매니저에게 넘겨주어서 렌더링을 하게 해준다.
class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

private:
	void AddParam(uint64 instanceId, InstancingParams& data);

private:
	map<uint64/*instanceId*/, shared_ptr<InstancingBuffer>> _buffers;
};

