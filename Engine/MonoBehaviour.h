#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();
public:

private:
	virtual void FinalUpdate() sealed {}//모노를 사용하는 것들이 해당 함수를 오버라이딩 해서 사용하지 못하도록
};

