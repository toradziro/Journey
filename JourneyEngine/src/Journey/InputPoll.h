#pragma once

#include "jnypch.h"
#include "Journey/Core.h"
#include "Journey/SingletonInterface.h"

namespace jny
{

struct MousePos
{
	float x = 0.0f;
	float y = 0.0f;
};

class  InputPoll : public ISingleton
{
	JNY_SINGLETON_TYPE(InputPoll)
public:
	~InputPoll() {}
	bool keyPressed(int keycode);
	bool mouseButtonPressed(int button);
	MousePos mousePos();
};

}