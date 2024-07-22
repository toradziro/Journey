#include "jnypch.h"
#include "SandboxApplication.h"

class ExampleLayer : public jny::Layer
{
public:

	ExampleLayer() : Layer("Test") { }

	void update() override
	{
		jny::Log::info("ExampleLayer::update");
	}

	void onEvent(jny::Event& event) override
	{
		jny::Log::trace(event.toString());
	}
};

Sandbox::Sandbox()
{
	this->pushLayer(new ExampleLayer());
	this->pushOverlay(new jny::ImGuiLayer());
}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
