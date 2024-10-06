#include "jnypch.h"

#include "Journey/Core/Application.h"
#include "Journey/Core/Core.h"
#include "Journey/Log/Log.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/ImGui/ImGuiLayer.h"
#include "Journey/Window/Window.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/RenderCommand.h"

#include "Journey/Core/Profiling/TimeInstruments.h"
#include "Journey/Core/Random.h"

namespace jny
{

std::unique_ptr<SingletonHolder> Application::s_sHolder;

Application::Application(const std::string_view& name)
{
	PROFILE_FUNC;

	s_sHolder = std::make_unique<SingletonHolder>();

	s_sHolder->add<Instrumentor>();
	s_sHolder->st<Instrumentor>().beginSession(C_PROFILE_INIT_FILE);

	s_sHolder->add<Random>();
	s_sHolder->add<VFS>();
	s_sHolder->st<VFS>().init();

	//-- Create main window
	s_sHolder->add<Window>(WindowData(name, 1200, 800));

	//-- Prepare to process events
	s_sHolder->st<Window>().setEventCallback([this](Event& _event)
		{
			onEvent(_event);
		});
	
	//-- Easy way to check if any events ongoing
	s_sHolder->add<InputPoll>();
	
	//-- Must be init before renderer is initialized
	s_sHolder->add<ShaderLibrary>();

	//-- Creating renderer based on API we want to use for rendering
	//-- RenderCommnad is a holder for API implementation and also caller for rendering methods
	s_sHolder->add<RenderCommand>();
	s_sHolder->st<RenderCommand>().createRenderer(RendererAPI::API::OpenGL);
	
	//-- Interface for rendering
	s_sHolder->add<Renderer>();
	s_sHolder->st<Renderer>().init();

	//-- And for 2D rendering as well
	s_sHolder->add<Renderer2D>();
	s_sHolder->st<Renderer2D>().init();

	m_imGuiLayer = pushOverlay<ImGuiLayer>();
	s_sHolder->st<Instrumentor>().endSession();
}

Application::~Application()
{
	s_sHolder->st<Renderer2D>().shutdown();
	s_sHolder.reset();
}

void Application::run()
{
	PROFILE_FUNC;

	s_sHolder->st<Instrumentor>().beginSession(C_PROFILE_MAIN_LOOP_FILE);

	std::chrono::duration<float> deltaTime = {};
	while (m_running)
	{
		PROFILE_SCOPE("Application::run::main_cicle");

		if (m_ciclingCount == C_PROFILING_FRAMES)
		{
			s_sHolder->st<Instrumentor>().endSession();
			s_sHolder->st<Instrumentor>().beginSession(C_PROFILE_MAIN_LOOP_FILE);

			m_ciclingCount = 0;
		}

		auto startTime = std::chrono::high_resolution_clock::now();

		if (!m_minimized)
		{
			std::ranges::for_each(m_layers, [&deltaTime](auto& layer)
				{
					layer->update(deltaTime.count());
				});
		}

		//-- ImGui drawing
		if (m_imGuiEnabled)
		{
			m_imGuiLayer->begin();
			std::ranges::for_each(m_layers, [](auto& layer)
				{
					layer->imGuiRender();
				});
			m_imGuiLayer->end();
		}

		s_sHolder->st<Window>().update();

		auto endTime = std::chrono::high_resolution_clock::now();
		deltaTime = endTime - startTime;

		++m_ciclingCount;
	}

	s_sHolder->st<Instrumentor>().endSession();
}

void Application::onEvent(Event& event)
{
	PROFILE_FUNC;

	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent&)
		{
			return windowCloseEvent();
		});

	dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e)
		{
			return windowResizeEvent(e);
		});

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		(*it)->onEvent(event);
		if (event.handeled())
		{
			break;
		}
	}
}

bool Application::windowCloseEvent()
{
	m_running = false;
	return true;
}

bool Application::windowResizeEvent(WindowResizeEvent& e)
{
	auto w = e.width();
	auto h = e.height();

	if (w == 0 && h == 0)
	{
		m_minimized = true;
		return false;
	}
	m_minimized = false;

	s_sHolder->st<Renderer>().windowResized(w, h);

	return false;
}

void Application::popLayer(Layer* layer)
{
	layer->detach();
	m_layers.popLayer(layer);
}

void Application::popOverlay(Layer* layer)
{
	layer->detach();
	m_layers.popOverlay(layer);
}

float Application::aspectRatio()
{
	return static_cast<float>(Application::subsystems().st<Window>().width()) /
		static_cast<float>(Application::subsystems().st<Window>().height());
}

} //-- jny