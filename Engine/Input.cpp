#include "pch.h"
#include "Input.h"


void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}



//각각의 움직임은 각가의 객체에서 알아서 한다.
//카메라 움직임은 TextCameraScript에서
void Input::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		//if (::GetAsyncKeyState(key) & 0x8000)
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
}