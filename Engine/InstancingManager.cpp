#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"


//이제 앞으로는 카메라에서 물체들을 렌더링 하는것이 아니라 
//인스턴스 매니저에게 넘겨주어서 렌더링을 하게 해준다.
void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	//같은 인스턴스id를 가진 물체들끼리 모아놓기 위한 map 자료구조
	map<uint64, vector<shared_ptr<GameObject>>> cache;

	//물체 분류
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		//하나일때
		if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->Render();
		}
		else//인스턴싱이 필요할때
		{
			const uint64 instanceId = pair.first;

			//한번에 그려주기 위해서 정보들을 만들어서 널어준다.
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
	//이미 만들어진 인스턴싱 버퍼가 있는지 확인하고 만들어져 있지 않으면 하나 새로 만들어서
	//데이터를 넣어준다.
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}