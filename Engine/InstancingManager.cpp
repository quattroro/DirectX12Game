#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"


//���� �����δ� ī�޶󿡼� ��ü���� ������ �ϴ°��� �ƴ϶� 
//�ν��Ͻ� �Ŵ������� �Ѱ��־ �������� �ϰ� ���ش�.
void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	//���� �ν��Ͻ�id�� ���� ��ü�鳢�� ��Ƴ��� ���� map �ڷᱸ��
	map<uint64, vector<shared_ptr<GameObject>>> cache;

	//��ü �з�
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		//�ϳ��϶�
		if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->Render();
		}
		else//�ν��Ͻ��� �ʿ��Ҷ�
		{
			const uint64 instanceId = pair.first;

			//�ѹ��� �׷��ֱ� ���ؼ� �������� ���� �ξ��ش�.
			for (const shared_ptr<GameObject>& gameObject : vec)
			{
				InstancingParams params;
				params.matWorld = gameObject->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWorld * Camera::S_MatView * Camera::S_MatProjection;

				AddParam(instanceId, params);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->Render(buffer);
		}
	}
}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers)
	{
		shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParam(uint64 instanceId, InstancingParams& data)
{
	//�̹� ������� �ν��Ͻ� ���۰� �ִ��� Ȯ���ϰ� ������� ���� ������ �ϳ� ���� ����
	//�����͸� �־��ش�.
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}