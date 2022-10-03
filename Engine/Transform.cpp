#include "pch.h"
#include "Transform.h"

Transform::Transform():Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{
}

void Transform::FinalUpdate()
{

}


void Transform::PushData()
{

}

//CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));