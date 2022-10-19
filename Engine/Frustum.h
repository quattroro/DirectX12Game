#pragma once

//절두체의 각 면들 넘버링
enum PLANE_TYPE : uint8
{
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_LEFT,
	PLANE_RIGHT,

	PLANE_END
};


//해당 친구는 카메라에서만 사용한다.
class Frustum
{
public:
	void FinalUpdate();
	bool ContainsSphere(const Vec3& pos, float radius);//안에 들어 있는지 확인

private:
	array<Vec4, PLANE_END> _planes;//벡터4 배열 6개
};

