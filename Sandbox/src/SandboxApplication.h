#pragma once

#include <Journey.h>

class Sandbox : public jny::Application
{
public:
	Sandbox();
	~Sandbox();

	void run() override;
};