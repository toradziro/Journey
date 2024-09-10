#include "jnypch.h"
#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

template <typename T>
void printTypeSize()
{
	jny::Log::error("'{}' : '{}'", typeid(T).name(), sizeof(T));
}

}

class Timer
{
public:
	using TimePoint = std::chrono::steady_clock::time_point;
	using TimeCallback = std::function<void(Sandbox2D::ProfileResult)>;

	Timer(std::string_view name, TimeCallback&& timerCallback) :
		m_timerCallback(timerCallback),
		m_name(name)
	{
		m_startPoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_stopped)
		{
			stop();
		}
	}

	void stop()
	{
		TimePoint endPoint = std::chrono::high_resolution_clock::now();
		int64_t timeTakenMcs = std::chrono::duration_cast<std::chrono::microseconds>(endPoint - m_startPoint).count();

		float duration = static_cast<float>(timeTakenMcs) * 0.001f;

		m_stopped = true;
		m_timerCallback({ m_name.data(), duration});
	}

private:
	TimeCallback			m_timerCallback;

	const std::string_view	m_name;
	TimePoint				m_startPoint;
	bool					m_stopped = false;
};

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2Ds"),
	m_orthoCameraCtrl(jny::Application::aspectRatio())
{ }

void Sandbox2D::attach()
{
	m_checkerboardTexture = jny::Texture2D::create("resources/assets/textures/checkerboard.png");
}

void Sandbox2D::detach() { }

void Sandbox2D::update(float dt)
{
	const Timer timer("Sandbox2D::update", [&](ProfileResult result)
		{
			m_profileData.push_back(result);
		});

	m_orthoCameraCtrl.update(dt);

	auto& rc = jny::Application::subsystems().st<jny::RenderCommand>();
	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	auto& renderer2D = jny::Application::subsystems().st<jny::Renderer2D>();

	//-- Start rendering
	renderer2D.beginScene(m_orthoCameraCtrl.camera());
	renderer2D.drawQuad(m_pos, m_size, m_squareColor);
	renderer2D.drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture);

	//-- End rendering
	renderer2D.endScene();
}

void Sandbox2D::onEvent(jny::Event& event)
{
	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{
	ImGui::Begin("Color prop");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::DragFloat2("Position: ", glm::value_ptr(m_pos), 0.01f);
	ImGui::DragFloat2("Size: ", glm::value_ptr(m_size), 0.01f);

	std::ranges::for_each(m_profileData, [](const auto& data)
		{
			constexpr std::string_view C_PATTERN = "00.000 ms";

			const auto currCursor = ImGui::GetCursorPos();
			ImGui::Text("%.3f ms", data.m_time);
			ImGui::SetCursorPos(currCursor);
			ImGui::Dummy(ImVec2(ImGui::CalcTextSize(C_PATTERN.data()).x * ImGui::GetWindowDpiScale(), 0.0f));
			ImGui::SameLine();
			ImGui::TextUnformatted(data.m_name);
		});
	m_profileData.clear();

	ImGui::End();
}
