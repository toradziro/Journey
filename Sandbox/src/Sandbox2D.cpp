#include "jnypch.h"
#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

constexpr f32 C_SPEED_ROTATION = 1.0f;

}

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2Ds")
{ }

void Sandbox2D::attach()
{
}

void Sandbox2D::detach() { }

void Sandbox2D::update(float dt)
{
}

void Sandbox2D::onEvent(jny::Event& event)
{
}

void Sandbox2D::imGuiRender()
{
}
