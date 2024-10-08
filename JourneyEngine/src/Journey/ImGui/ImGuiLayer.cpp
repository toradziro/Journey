#include "jnypch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Journey/Core/Application.h"
#include "Journey/Window/Window.h"

#include "Journey/Events/Event.h"
#include "Journey/Events/MouseEvent.h"
#include "Journey/Events/KeyEvent.h"
#include "Journey/Core/Profiling/TimeInstruments.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

namespace
{

constexpr std::string_view C_CONFIG_FILE_NAME = "imgui.ini";
constexpr std::string_view C_DEFAULT_LAYOUT_PATH = "editor/default_layout.ini";

}

namespace jny
{

ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::attach()
{
	PROFILE_FUNC;

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	ImGui::StyleColorsDark();
	
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto& win = Application::subsystems().st<Window>();

	ImGui::GetStyle().ScaleAllSizes(win.dpiScale());
	float fontSize = 17.0f * win.dpiScale();

	io.Fonts->Clear();

	ImFontConfig fontConfig;
	fontConfig.OversampleH = 2;
	fontConfig.OversampleV = 1;
	fontConfig.PixelSnapH = true;

	//io.Fonts->AddFontFromFileTTF("../resources/fonts/noto_fonts.ttf", fontSize, &fontConfig);
	io.Fonts->AddFontFromFileTTF("../resources/fonts/roboto.ttf", fontSize, &fontConfig);

	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(win.rawWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 410");

	if (!std::filesystem::exists(C_CONFIG_FILE_NAME))
	{
		auto& vfs = Application::subsystems().st<VFS>();
		Ref<File> defaultLayout = vfs.loadFile(C_DEFAULT_LAYOUT_PATH);
		std::string layoutAsStr = defaultLayout->toString();
		ImGui::LoadIniSettingsFromMemory(layoutAsStr.c_str(), layoutAsStr.size());
	}
}

void ImGuiLayer::detach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event& e)
{
	if (m_markEventsProcessed)
	{
		ImGuiIO& io = ImGui::GetIO();
		EventDispatcher dispatcher(e);
		if (io.WantCaptureMouse)
		{
			dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent&)
				{
					return true;
				});
			dispatcher.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent&)
				{
					return true;
				});
			dispatcher.dispatch<MouseMovedEvent>([this](MouseMovedEvent&)
				{
					return true;
				});
			dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent&)
				{
					return true;
				});
		}
		if (io.WantCaptureKeyboard)
		{
			dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent&)
				{
					return true;
				});
			dispatcher.dispatch<KeyReleasedEvent>([this](KeyReleasedEvent&)
				{
					return true;
				});
			dispatcher.dispatch<KeyTypedEvent>([this](KeyTypedEvent&)
				{
					return true;
				});
		}
	}
}

void ImGuiLayer::imGuiRender()
{
	static bool s_show = false;
	if (s_show)
	{
		ImGui::ShowDemoWindow();
	}
}

void ImGuiLayer::begin()
{
	PROFILE_FUNC;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end()
{
	PROFILE_FUNC;

	ImGuiIO& io = ImGui::GetIO();
	auto& win = Application::subsystems().st<Window>();

	io.DisplaySize = ImVec2((float)win.width(), (float)win.height());

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		auto* context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(context);
	}
}

} //-- jny