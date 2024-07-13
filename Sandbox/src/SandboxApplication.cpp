#include "SandboxApplication.h"

Sandbox::Sandbox()
{

}

Sandbox::~Sandbox()
{

}

void Sandbox::run()
{
	jny::Log::log(jny::Log::LogLevel::info, "Sandbox::run {}", "start");
}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
