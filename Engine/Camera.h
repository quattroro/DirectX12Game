#pragma once
#include "Component.h"
#include "Frustum.h"
enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE GetProjectionType() { return _type; }

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;

	//카메라는 여러개의 레이어를 찍어야 할 수도 있기 때문에 찍어야 될 레이어 정보를 비트 플래그로 가지고 있는다.
	uint32 _cullingMask = 0;

public:
	//레이어 관련 함수들
	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }//모든비트를 1로 채운다 -> 아무것도 찍지 않는다.
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

public:
	// TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;

	
};

