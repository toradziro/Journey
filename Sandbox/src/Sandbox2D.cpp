#include "jnypch.h"
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2Ds"),
	m_orthoCameraCtrl(jny::Application::aspectRatio())
{ }

void Sandbox2D::attach()
{

}

void Sandbox2D::detach()
{

}

void Sandbox2D::update(float dt)
{

}

void Sandbox2D::onEvent(jny::Event& event)
{
	m_orthoCameraCtrl.onEvent(event);
}

void Sandbox2D::imGuiRender()
{

}

