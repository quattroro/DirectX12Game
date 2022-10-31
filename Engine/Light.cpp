#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Resources.h"


Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();
}

void Light::Render()
{
	assert(_lightIndex >= 0);

	GetTransform()->PushData();//볼륨 메쉬를 이용해 자신의 영역을 표시 하는데 그 메쉬 데이터를 넘겨주는 작업

	_lightMaterial->SetInt(0, _lightIndex);
	_lightMaterial->PushGraphicsData();//마테리얼 정보를 넘겨준다.

	//라이트 타입에 따라 설정
	switch (static_cast<LIGHT_TYPE>(_lightInfo.lightType))
	{
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * _lightInfo.range;//반지름이 0.5이기 때문에
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	_volumeMesh->Render();
}


//라이트 타입에 따라 리소스들을 받아온다.
void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	}
}