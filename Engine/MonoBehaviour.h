#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();
public:

private:
	virtual void FinalUpdate() sealed {}//��븦 ����ϴ� �͵��� �ش� �Լ��� �������̵� �ؼ� ������� ���ϵ���
};

