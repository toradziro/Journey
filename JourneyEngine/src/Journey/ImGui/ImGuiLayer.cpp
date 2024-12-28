#include "jnypch.h"
#include "ImGuiLayer.h"


#include "Journey/Core/Application.h"
#include "Journey/Window/Window.h"

#include "Journey/Events/Event.h"
#include "Journey/Events/MouseEvent.h"
#include "Journey/Events/KeyEvent.h"
#include "Journey/Core/Profiling/TimeInstruments.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <ImGuizmo.h>

namespace
{

constexpr std::string_view	C_CONFIG_FILE_NAME = "imgui.ini";
constexpr std::string_view	C_DEFAULT_LAYOUT_PATH = "editor/default_layout.ini";
constexpr const char*		C_REGULAR_FONTS = "../resources/fonts/open_sans.ttf";
constexpr const char*		C_PIXEL_FONTS = "../resources/fonts/pixel_font.ttf";

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
	//-- WinBg
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	style.Colors[ImGuiCol_WindowBg].x = 0.11f;
	style.Colors[ImGuiCol_WindowBg].y = 0.11f;
	style.Colors[ImGuiCol_WindowBg].z = 0.11f;
	//-- FrameBg
	style.Colors[ImGuiCol_FrameBg].x = 0.25f;
	style.Colors[ImGuiCol_FrameBg].y = 0.25f;
	style.Colors[ImGuiCol_FrameBg].z = 0.25f;
	//-- FrameBgHovered
	style.Colors[ImGuiCol_FrameBgHovered].x = 0.5f;
	style.Colors[ImGuiCol_FrameBgHovered].y = 0.5f;
	style.Colors[ImGuiCol_FrameBgHovered].z = 0.5f;
	//-- FrameBgActive
	style.Colors[ImGuiCol_FrameBgActive].x = 0.8f;
	style.Colors[ImGuiCol_FrameBgActive].y = 0.8f;
	style.Colors[ImGuiCol_FrameBgActive].z = 0.8f;
	//-- TitleBg
	style.Colors[ImGuiCol_TitleBg].x = 0.15f;
	style.Colors[ImGuiCol_TitleBg].y = 0.15f;
	style.Colors[ImGuiCol_TitleBg].z = 0.15f;
	//-- TitleBgActive
	style.Colors[ImGuiCol_TitleBgActive].x = 0.3f;
	style.Colors[ImGuiCol_TitleBgActive].y = 0.3f;
	style.Colors[ImGuiCol_TitleBgActive].z = 0.3f;
	//-- Header
	style.Colors[ImGuiCol_Header].x = 0.4f;
	style.Colors[ImGuiCol_Header].y = 0.4f;
	style.Colors[ImGuiCol_Header].z = 0.4f;
	//-- Header Hovered
	style.Colors[ImGuiCol_HeaderHovered].x = 0.56f;
	style.Colors[ImGuiCol_HeaderHovered].y = 0.56f;
	style.Colors[ImGuiCol_HeaderHovered].z = 0.56f;
	//-- Tab
	style.Colors[ImGuiCol_Tab].x = 0.4f;
	style.Colors[ImGuiCol_Tab].y = 0.4f;
	style.Colors[ImGuiCol_Tab].z = 0.4f;
	//-- TabHovered
	style.Colors[ImGuiCol_TabHovered].x = 0.56f;
	style.Colors[ImGuiCol_TabHovered].y = 0.56f;
	style.Colors[ImGuiCol_TabHovered].z = 0.56f;
	//-- TabSelected
	style.Colors[ImGuiCol_TabSelected].x = 0.6f;
	style.Colors[ImGuiCol_TabSelected].y = 0.6f;
	style.Colors[ImGuiCol_TabSelected].z = 0.6f;

	style.Colors[ImGuiCol_TabSelectedOverline].x = 0.6f;
	style.Colors[ImGuiCol_TabSelectedOverline].y = 0.6f;
	style.Colors[ImGuiCol_TabSelectedOverline].z = 0.6f;

	style.Colors[ImGuiCol_TabDimmedSelected].x = 0.28f;
	style.Colors[ImGuiCol_TabDimmedSelected].y = 0.28f;
	style.Colors[ImGuiCol_TabDimmedSelected].z = 0.28f;

	style.Colors[ImGuiCol_Button].x = 0.555f;
	style.Colors[ImGuiCol_Button].y = 0.555f;
	style.Colors[ImGuiCol_Button].z = 0.555f;

	auto& win = Application::subsystems().st<Window>();

	ImGui::GetStyle().ScaleAllSizes(win.dpiScale());
	float regularFontSize = 18.0f * win.dpiScale();
	float pixelFontSize = 16.0f * win.dpiScale();

	io.Fonts->Clear();

	ImFontConfig fontConfig;
	fontConfig.OversampleH = 2;
	fontConfig.OversampleV = 1;
	fontConfig.PixelSnapH = true;

	m_regularFont = io.Fonts->AddFontFromFileTTF(C_REGULAR_FONTS, regularFontSize, &fontConfig);
	m_pixelFont = io.Fonts->AddFontFromFileTTF(C_PIXEL_FONTS, pixelFontSize, &fontConfig);

	m_currFont = m_pixelFont;

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

void ImGuiLayer::setFont(Font font)
{
	switch (font)
	{
	case Font::Pixel:
		m_currFont = m_pixelFont;
		break;
	case Font::Regular:
		m_currFont = m_regularFont;
		break;
	default:
		break;
	}
}

void ImGuiLayer::begin()
{
	PROFILE_FUNC;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(m_currFont);

	ImGuizmo::BeginFrame();
}

void ImGuiLayer::end()
{
	PROFILE_FUNC;

	ImGuiIO& io = ImGui::GetIO();
	auto& win = Application::subsystems().st<Window>();

	io.DisplaySize = ImVec2((float)win.width(), (float)win.height());

	ImGui::PopFont();
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