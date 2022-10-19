#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

//�� ������ ���� ����
void Frustum::FinalUpdate()
{
	//����ü�� ����� ���ϱ� ���� �������� projection��ǥ���� ���ϰ� �� ����� ���࿭���� �����༭ ���忡���� ��ǥ�� ��´�
	Matrix matViewInv = Camera::S_MatView.Invert();
	Matrix matProjectionInv = Camera::S_MatProjection.Invert();
	Matrix matInv = matProjectionInv * matViewInv;

	//world ��ǥ���� ũ�Ⱑ 1�� ������ü�� �����
	vector<Vec3> worldPos =
	{
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),//�ش��Լ��� ����� ���� ������ ����Ŭ����ǥ�� ����(w ������ 1�� ����)
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv)
	};

	//�ش� �Լ��� �̿��ؼ� ����ü�� ���� �����
	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]); // CW
	/*���ο����� ������ ���� 3���� �̿��ؼ� ���� �ΰ��� �����
	  �ش� ���� �ΰ��� �����ؼ� �븻�� ����� n(a,b,c)
	  �׸��� �ش� �븻�� ���� �ϳ��� �����ؼ� d�� ����� �ָ� 
	  ����� �������� ���� ��  �ִ�.*/
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]); // CCW
	_planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]); // CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]); // CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]); // CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]); // CCW
}

bool Frustum::ContainsSphere(const Vec3& pos/*��ü�� ����*/, float radius/*ũ��*/)
{
	/*BoundingFrustum �� �̿��� directX���� �⺻������ �����ϴ� Frustum Culling�� ��� �� �� �ִ�.
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