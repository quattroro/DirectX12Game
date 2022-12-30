#pragma once
#include "InstancingBuffer.h"

class GameObject;



//�ν��Ͻ� ���۴� ������ ��ü ��Ʈ ���� �ϳ��� �ʿ��ϱ� ������ �ش� �ν��Ͻ����� �������ִ� ������ �� �Ŵ��� �̴�.
//���� �����δ� ī�޶󿡼� ��ü���� ������ �ϴ°��� �ƴ϶� 
//�ν��Ͻ� �Ŵ������� �Ѱ��־ �������� �ϰ� ���ش�.
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

