#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "Resources.h"
void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::Render()
{
	//조명에 대한 정보들을 여기서 만들어 준다.
	PushLightData();

	//커매드 큐의 렌더 비긴에 있던 것이 여기로 왔다.
	// SwapChain Group 초기화
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);

	// Deferred Group 초기화
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	// Lighting Group 초기화
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	// Deferred OMSet 
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	//가장 처음 만들어진 카메라를 메인 카메라로 한다.
	shared_ptr<Camera> mainCamera = _cameras[0];
	mainCamera->SortGameObject();
	mainCamera->Render_Deferred();


	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();//타겟에서 리소스로
	RenderLights();

	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();//타겟에서 리소스로
	RenderFinal();


	mainCamera->Render_Forward();


	//일부는 디퍼드, 일부는 포워드로 그려야 하는는데 
	//그러기 위해서 미리 카메라 쪽에서 정렬을 시켜서 따로따로 렌더를 한다. 
	for (auto& camera : _cameras)
	{
		//메인 카메라를 제외한 나머지 카메라들은 소팅을 하고 포워드 렌더만 한다.
		if (camera == mainCamera)
			continue;

		camera->SortGameObject();
		camera->Render_Forward();
	}



	//for (auto& gameObject : _gameObjects)
	//{
	//	if (gameObject->GetCamera() == nullptr)
	//		continue;

	//	// 여기서 디퍼드인지 포워드 인지에 따라 소팅을 하고
	//	gameObject->GetCamera()->SortGameObject();

	//	//포지션, 노말, 컬러 값을 지정해주고
	//	// Deferred OMSet
	//	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	//	gameObject->GetCamera()->Render_Deferred();

	//	// Light OMSet

	//	// Swapchain OMSet
	//	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
	//	gameObject->GetCamera()->Render_Forward();
	//}
}

//광원을 하나씩 그린다.
void Scene::RenderLights()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	// 광원을 그린다.
	for (auto& light : _lights)
	{
		light->Render();
	}
}

void Scene::RenderFinal()
{
	// Swapchain OMSet
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	GET_SINGLE(Resources)->Get<Material>(L"Final")->PushData();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}

void Scene::PushLightData()
{
	LightParams lightParams = {};

	for (auto& light : _lights)
	{
		const LightInfo& lightInfo = light->GetLightInfo();

		//자신이 몇번째 라이트인지 를 세팅해준다.
		light->SetLightIndex(lightParams.lightCount);

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount++;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	//카메라와 라이트는 다른 게임오브젝으와 따로 관리 한다.
	if (gameObject->GetCamera() != nullptr)
	{
		_cameras.push_back(gameObject->GetCamera());
	}
	else if (gameObject->GetLight() != nullptr)
	{
		_lights.push_back(gameObject->GetLight());
	}

	_gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameras.begin(), _cameras.end(), gameObject->GetCamera());
		if (findIt != _cameras.end())
			_cameras.erase(findIt);
	}
	else if (gameObject->GetLight())
	{
		auto findIt = std::find(_lights.begin(), _lights.end(), gameObject->GetLight());
		if (findIt != _lights.end())
			_lights.erase(findIt);
	}

	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
		_gameObjects.erase(findIt);
}