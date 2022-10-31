#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "TestCameraScript.h"
#include "Resources.h"

//엔진에서 UI를 표시 할때에는 UI만 따로 찍는 카메라가 필요함
//그런데 이렇게 여러개의 카메라를 쏘기 위해서는 UI인지 아닌지 판별하기위한 장치가 필요한데
//그것이 Layer이다


void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

// TEMP
void SceneManager::Render()
{
	if (_activeScene != nullptr)
		_activeScene->Render();

	/*const vector<shared_ptr<GameObject>>& gameObjects = _activeScene->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->Render();
	}*/
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}



shared_ptr<Scene> SceneManager::LoadTestScene()
{
	

	//#pragma region TestObject
	//	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	//
	//	vector<Vertex> vec(4);
	//	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	//	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	//	vec[0].uv = Vec2(0.f, 0.f);
	//	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	//	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	//	vec[1].uv = Vec2(1.f, 0.f);
	//	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	//	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	//	vec[2].uv = Vec2(1.f, 1.f);
	//	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	//	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	//	vec[3].uv = Vec2(0.f, 1.f);
	//
	//	vector<uint32> indexVec;
	//	{
	//		indexVec.push_back(0);
	//		indexVec.push_back(1);
	//		indexVec.push_back(2);
	//	}
	//	{
	//		indexVec.push_back(0);
	//		indexVec.push_back(2);
	//		indexVec.push_back(3);
	//	}
	//
	//	gameObject->AddComponent(make_shared<Transform>());
	//	shared_ptr<Transform> transform = gameObject->GetTransform();
	//	transform->SetLocalPosition(Vec3(0.f, 100.f, 200.f));
	//	transform->SetLocalScale(Vec3(100.f, 100.f, 1.f));
	//
	//	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	//	{
	//		shared_ptr<Mesh> mesh = make_shared<Mesh>();
	//		mesh->Init(vec, indexVec);
	//		meshRenderer->SetMesh(mesh);
	//	}
	//	{
	//		shared_ptr<Shader> shader = make_shared<Shader>();
	//		shared_ptr<Texture> texture = make_shared<Texture>();
	//		shader->Init(L"..\\Resources\\Shader\\default.fx");
	//		texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
	//		shared_ptr<Material> material = make_shared<Material>();
	//		material->SetShader(shader);
	//		material->SetFloat(0, 0.3f);
	//		material->SetFloat(1, 0.4f);
	//		material->SetFloat(2, 0.3f);
	//		material->SetTexture(0, texture);
	//		meshRenderer->SetMaterial(material);
	//	}
	//	gameObject->AddComponent(meshRenderer);
	//	scene->AddGameObject(gameObject);
	//#pragma endregion

		//사용할 레이어들 정의
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion

	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));

		//1번 카메라는 UI는 찍지 않는다
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음

		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion


#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");


			/*shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\skybox.fx",
				{ RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
			texture->Init(L"..\\Resources\\Texture\\Sky01.jpg");*/
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Object
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, texture2);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion



#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, 0, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 0.f, 0.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		scene->AddGameObject(light);
	}
#pragma endregion

//#pragma region Point Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetLightRange(200.f);
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//
//#pragma region Spot Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetLightRange(200.f);
//		light->GetLight()->SetLightAngle(3.14f / 2);
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion


//#pragma region Green Directional Light
//	{
//		//초록색 directional light
//		//위에서 아래로 
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 1.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
//		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
//
//		scene->AddGameObject(light);
//	}
//
//#pragma endregion

//#pragma region Cube
//	{
//		shared_ptr<GameObject> sphere = make_shared<GameObject>();
//		sphere->AddComponent(make_shared<Transform>());
//		sphere->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		sphere->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
//			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
//
//			/*shared_ptr<Shader> shader = make_shared<Shader>();
//			shared_ptr<Texture> texture = make_shared<Texture>();
//			shared_ptr<Texture> texture2 = make_shared<Texture>();
//			shader->Init(L"..\\Resources\\Shader\\default.fx");
//			texture->Init(L"..\\Resources\\Texture\\Leather.jpg");
//			texture2->Init(L"..\\Resources\\Texture\\Leather_Normal.jpg");*/
//
//
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			material->SetTexture(1, texture2);
//			meshRenderer->SetMaterial(material);
//		}
//		sphere->AddComponent(meshRenderer);
//		scene->AddGameObject(sphere);
//	}
//#pragma endregion


//#pragma region Red Point Light
//	{
//		//
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(150.f, 150.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		//light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(1.f, 0.1f, 0.1f));
//		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetLightRange(10000.f);
//		//light->GetLight()->SetLightAngle(XM_PI / 4);
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//
//#pragma region Blue Spot Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(-150.f, 0.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.f, 0.1f, 1.f));
//		//light->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetLightRange(10000.f);
//		light->GetLight()->SetLightAngle(XM_PI / 4);//각도는 45도
//		scene->AddGameObject(light);
//	}
//#pragma endregion

//#pragma region Cube
//	{
//		shared_ptr<GameObject> sphere = make_shared<GameObject>();
//		sphere->AddComponent(make_shared<Transform>());
//		sphere->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		sphere->GetTransform()->SetLocalPosition(Vec3(150.f, 100.f, 200.f));
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = make_shared<Shader>();
//			shared_ptr<Texture> texture = make_shared<Texture>();
//			shader->Init(L"..\\Resources\\Shader\\default.fx");
//			texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		sphere->AddComponent(meshRenderer);
//		scene->AddGameObject(sphere);
//	}
//#pragma endregion


	return scene;
}