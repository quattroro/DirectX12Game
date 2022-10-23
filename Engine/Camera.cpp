#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Material.h"
#include "Shader.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();//월드행렬의 역행렬을 구한다.=> 뷰 변환 행렬이 된다.

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);


	_frustum.FinalUpdate();
}

//void Camera::Render()
//{
//	S_MatView = _matView;
//	S_MatProjection = _matProjection;
//
//
//	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
//
//	// TODO : Layer 구분
//	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();//씬에 있는 모든 오브젝트들을 받아온다.
//
//	for (auto& gameObject : gameObjects)
//	{
//		if (gameObject->GetMeshRenderer() == nullptr)
//			continue;
//
//		//현재 게임 오브젝트의 레이어가 내가 그려야 하는 레이어 인지 아닌지 확인
//		if (IsCulled(gameObject->GetLayerIndex()))
//			continue;
//
//		//frustum컬링을 적용할 개체인지 아닌지 적용할 수있도록 GameObject에 있다.
//		//컬링을 진행하지 않고 항상 화면에 보여야 하는 물체가 있기 때문
//		if (gameObject->GetCheckFrustum())
//		{
//
//			if (_frustum.ContainsSphere(
//				gameObject->GetTransform()->GetWorldPosition(),
//				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
//			{
//				continue;
//			}
//		}
//
//		gameObject->GetMeshRenderer()->Render();
//	}
//}


void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();

	//모든 게임 오브젝트들을 순회하면서 내가 그려줘야 하는 친구 인지 확인한다.
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		//컬링이 되어 있는지 체크
		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		//그 킨구들의 셰이더 타입을 확인해서 각각 넣어준다.
		SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
		switch (shaderType)
		{
		case SHADER_TYPE::DEFERRED:
			_vecDeferred.push_back(gameObject);
			break;
		case SHADER_TYPE::FORWARD:
			_vecForward.push_back(gameObject);
			break;
		}
	}
}

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& gameObject : _vecDeferred)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& gameObject : _vecForward)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}