#include "SandboxApplication.h"

Sandbox::Sandbox()
{

}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
