#pragma once

#include "Journey/Layer/Layer.h"

struct ImFont;

namespace jny
{

class ImGuiLayer : public Layer
{
public:
	enum class Font : u8
	{
		Pixel,
		Regular
	};

	ImGuiLayer();
	~ImGuiLayer();

	void attach() override;
	void detach() override;
	void onEvent(Event& e) override;
	void imGuiRender() override;

	void setFont(Font font);

	void setUpMarkEventsProcessed(bool condition) { m_markEventsProcessed = condition; }

	void begin();
	void end();

private:
	ImFont*		m_pixelFont = nullptr;
	ImFont*		m_regularFont = nullptr;
	ImFont*		m_currFont = nullptr;
	float		m_time = 0.0f;
	bool		m_markEventsProcessed = true;
};

} //-- jny

