#pragma once

#include "Journey/Layer/Layer.h"

namespace jny
{

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void attach() override;
	void detach() override;
	void onEvent(Event& e) override;
	void imGuiRender() override;

	void setUpMarkEventsProcessed(bool condition) { m_markEventsProcessed = condition; }

	void begin();
	void end();

private:
	float	m_time = 0.0f;
	bool	m_markEventsProcessed = true;
};

} //-- jny

