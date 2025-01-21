#include "jnypch.h"
#include "MainHeroSystem.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/Scene/Components.h"
#include <GLFW/glfw3.h>

namespace jny
{

void MainHeroSystem::update(f32 dt)
{
	auto& inputPoll = Application::subsystems().st<jny::InputPoll>();

	m_registry.view<MainHeroComponent>().each([&](entt::entity e, MainHeroComponent& mhc)
		{
			f32 characterPosX = 0.0f;
			f32 characterPosY = 0.0f;

			TransformComponent& tc = m_registry.get<TransformComponent>(e);
			auto& pos = tc.m_position;

			const f32 moveDelta = mhc.m_movementSpeed * dt;
			if (inputPoll.keyPressed(GLFW_KEY_W))
			{
				pos.y += moveDelta;
			}
			if (inputPoll.keyPressed(GLFW_KEY_S))
			{
				pos.y -= moveDelta;
			}
			if (inputPoll.keyPressed(GLFW_KEY_D))
			{
				pos.x += moveDelta;
			}
			if (inputPoll.keyPressed(GLFW_KEY_A))
			{
				pos.x -= moveDelta;
			}
			characterPosX = pos.x;
			characterPosY = pos.y;

			m_registry.view<CameraComponent>().each([&](entt::entity e, CameraComponent& cc)
				{
					if (cc.m_primer)
					{
						auto& cameraPos = m_registry.get<TransformComponent>(e).m_position;
						cameraPos.x = characterPosX;
						cameraPos.y = characterPosY;
					}
				});
		});
}

}