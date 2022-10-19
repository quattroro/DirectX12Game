#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

//매 프레임 마다 실행
void Frustum::FinalUpdate()
{
	//절두체의 평면을 구하기 위한 정점들을 projection좌표에서 구하고 각 행렬의 역행열들을 곱해줘서 월드에서의 좌표를 얻는다
	Matrix matViewInv = Camera::S_MatView.Invert();
	Matrix matProjectionInv = Camera::S_MatProjection.Invert();
	Matrix matInv = matProjectionInv * matViewInv;

	//world 좌표에서 크기가 1인 정육면체를 만든다
	vector<Vec3> worldPos =
	{
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),//해당함수로 계산을 해준 이유는 동차클립좌표계 때문(w 성분을 1로 세팅)
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv)
	};

	//해당 함수를 이용해서 절두체의 면을 만든다
	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]); // CW
	/*내부에서는 전해준 정점 3개를 이용해서 벡터 두개를 만들고
	  해당 벡터 두개를 외적해서 노말을 만든다 n(a,b,c)
	  그리고 해당 노말과 정점 하나를 내적해서 d를 만들어 주면 
	  평면의 방정식을 얻을 수  있다.*/
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]); // CCW
	_planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]); // CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]); // CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]); // CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]); // CCW
}

bool Frustum::ContainsSphere(const Vec3& pos/*물체의 원점*/, float radius/*크기*/)
{
	/*BoundingFrustum 을 이용해 directX에서 기본적으로 제공하는 Frustum Culling을 사용 할 수 있다.
	*/

	for (const Vec4& plane : _planes)
	{
		// n = (a, b, c)
		Vec3 normal = Vec3(plane.x, plane.y, plane.z);

		// ax + by + cz + d > radius
		if (normal.Dot(pos) + plane.w > radius)
			return false;
	}

	return true;
}