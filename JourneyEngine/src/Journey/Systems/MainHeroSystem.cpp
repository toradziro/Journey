#include "jnypch.h"
#include "MainHeroSystem.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/Scene/Components.h"
#include "Journey/Window/Window.h"
#include "Journey/Renderer/Renderer2D.h"
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

			glm::vec3 lineStartPos = {};
			glm::vec3 lineEndPos = {};

			TransformComponent& tc = m_registry.get<TransformComponent>(e);
			auto& pos = tc.m_position;
			lineStartPos = pos;

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
			auto mousePos = inputPoll.mousePos();
			lineEndPos.x = mousePos.x;
			lineEndPos.y = mousePos.y;

			characterPosX = pos.x;
			characterPosY = pos.y;

			m_registry.view<CameraComponent>().each([&](entt::entity e, CameraComponent& cc)
				{
					if (cc.m_primer)
					{
						auto& cameraPos = m_registry.get<TransformComponent>(e).m_position;
						auto camTransform = m_registry.get<TransformComponent>(e).transform();

						cameraPos.x = characterPosX;
						cameraPos.y = characterPosY;

						//f32 screenWidth = static_cast<f32>(cc.m_viewportH);
						//f32 screenHeight = static_cast<f32>(cc.m_viewportW);

						//glm::mat4 invProjection = glm::inverse(cc.projection());

						//glm::vec4 clipCoords = {};
						//clipCoords.x = (2.0f * lineEndPos.x) / screenWidth - 1.0f;
						//clipCoords.y = 1.0f - (2.0f * lineEndPos.y) / screenHeight;
						//clipCoords.z = 0.0f;
						//clipCoords.w = 1.0f;

						//glm::vec4 worldCoords = invProjection * clipCoords;

						//lineEndPos = glm::vec3(worldCoords) + cameraPos;

						//auto& renderer = Application::subsystems().st<Renderer2D>();
						//renderer.beginScene(cc, camTransform);
						//renderer.drawLine({{1.0f, 0.0f, 0.0f, 1.0f}, -1, lineStartPos, lineEndPos });
						//renderer.endScene();
					}
				});
		});
}

}