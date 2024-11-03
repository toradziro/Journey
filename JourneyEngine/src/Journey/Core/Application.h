#pragma once

#include "Journey/Core/Core.h"
#include "Journey/Layer/LayerStack.h"
#include "SingletonInterface.h"

namespace jny
{

class ImGuiLayer;
class Window;
class WindowResizeEvent;

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  Application
{
	static constexpr u8		C_PROFILING_FRAMES = 20;
	static constexpr const char*	C_PROFILE_INIT_FILE = "init.json";
	static constexpr const char*	C_PROFILE_MAIN_LOOP_FILE = "main_loop.json";

public:
	Application(const std::string_view name = "Journey");
	virtual ~Application();

	void run();

	void onEvent(Event& event);
	bool windowCloseEvent();
	bool windowResizeEvent(WindowResizeEvent& e);

	template<typename T, typename ...Args>
	T* pushLayer(Args&&... args)
	{
		return m_layers.pushLayer<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	T* pushOverlay(Args&&... args)
	{
		return m_layers.pushOverlay<T>(std::forward<Args>(args)...);
	}

	void popLayer(Layer* layer);
	void popOverlay(Layer* layer);

	void disableImGui() { m_imGuiEnabled = false; }
	void enableImGui() { m_imGuiEnabled = true; }
	ImGuiLayer* imGuiLayer() { return m_imGuiLayer; }

	static float aspectRatio();
	static inline SingletonHolder& subsystems() { JNY_ASSERT(s_sHolder.get() != nullptr, "Don't know this systyem"); return *s_sHolder; }

private:
	LayerStack	m_layers;
	ImGuiLayer*	m_imGuiLayer = nullptr;
	u8		m_ciclingCount = 0;
	bool		m_running = true;
	bool		m_minimized = false;
	bool		m_imGuiEnabled = true;

private:
	static std::unique_ptr<SingletonHolder>	s_sHolder;
};

//-- Will be defined in client
Application* createApplication();

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

} //-- jny
