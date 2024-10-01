#include "jnypch.h"
#include "SandboxApplication.h"
#include "Sandbox2D.h"

#include "Journey/Core/InputPoll.h"
#include "Journey/Core/EntryPoint.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Sandbox::Sandbox()
{
	disableImGui();
	this->pushLayer<Sandbox2D>();
}

Sandbox::~Sandbox()
{

}

jny::Application* jny::createApplication()
{
	return new Sandbox();
}
