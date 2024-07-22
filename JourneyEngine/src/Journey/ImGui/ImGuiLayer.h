#pragma once

#include "Journey/Layer/Layer.h"

namespace jny
{

class JNY_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void onEvent(Event& event) override;
	void attach() override;
	void detach() override;
	void update() override;

private:
	float m_time = 0.0f;
};

} //-- jny

