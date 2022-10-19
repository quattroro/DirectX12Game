#pragma once

//����ü�� �� ��� �ѹ���
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


//�ش� ģ���� ī�޶󿡼��� ����Ѵ�.
class Frustum
{
public:
	void FinalUpdate();
	bool ContainsSphere(const Vec3& pos, float radius);//�ȿ� ��� �ִ��� Ȯ��

private:
	array<Vec4, PLANE_END> _planes;//����4 �迭 6��
};

