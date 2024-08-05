#include "common.h"

#include "SandboxApplication.h"
#include "Journey/InputPoll.h"

#include <imgui.h>

class ExampleLayer : public jny::Layer
{
public:

	ExampleLayer() : Layer("Test") { }

	void update() override
	{
	}

	void onEvent(jny::Event& event) override
	{
		if (event.eventType() == jny::Event::EventType::KeyPressed)
		{
			auto mousePos = jny::Application::subsystems().st<jny::InputPoll>().mousePos();
			jny::Log::trace("{} {}", mousePos.x, mousePos.y);
		}
	}

	void imGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("hello from new layer method");
		ImGui::End();
	}
};

Sandbox::Sandbox()
{
	auto* exampleLayer = this->pushLayer<ExampleLayer>();
}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
