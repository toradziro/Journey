#include "jnypch.h"
#include "ParticleSystem.h"

#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace jny
{

ParticleSystem::ParticleSystem()
{
	m_particlePool.resize(1000);
}

void ParticleSystem::update(float dt)
{
	std::ranges::for_each(m_particlePool, [&](auto& particle)
		{
			if (particle.m_active)
			{
				if (particle.m_lifeRemaning <= 0.0f)
				{
					particle.m_active = false;
					return;
				}

				particle.m_lifeRemaning -= dt;
				particle.m_lifeRemaning -= dt;
				particle.m_position += particle.m_velocity * dt;
				particle.m_rotation += 0.01f * dt;
			}
		});
}

void ParticleSystem::render(const OrthographicCamera& camera)
{
	auto& renderer = Application::subsystems().st<Renderer2D>();

	std::ranges::for_each(m_particlePool, [&](auto& particle)
		{
			if (particle.m_active)
			{
				const float lifePoint = particle.m_lifeRemaning / particle.m_lifeTime;
				QuadCfg rendererCfg;
				rendererCfg.m_rotateOpt = RotateOpt::Rotated;
				rendererCfg.m_color = glm::lerp(particle.m_colorBegin, particle.m_colorEnd, lifePoint);
				rendererCfg.m_rotation = particle.m_rotation;
				rendererCfg.m_position = { particle.m_position, 0.0f };
				const float lifetimeSize = glm::lerp(particle.m_sizeBegin, particle.m_sizeEnd, lifePoint);
				rendererCfg.m_size = { lifetimeSize, lifetimeSize, 0.0f };

				renderer.drawQuad(rendererCfg);
			}
		});
}

void ParticleSystem::emit(const ParticleProps& props)
{
	auto& randSt = Application::subsystems().st<Random>();

	Particle& p = m_particlePool[m_poolIndex];
	p.m_active = true;
	p.m_position = props.m_position;
	p.m_rotation = randSt.genFloat() * 2.0f * glm::pi<float>();

	p.m_velocity = props.m_velocity;
	p.m_velocity.x += props.m_velocityVariation.x * (randSt.genFloat() - 0.5f);
	p.m_velocity.y += props.m_velocityVariation.y * (randSt.genFloat() - 0.5f);

	p.m_colorBegin = props.m_colorBegin;
	p.m_colorEnd = props.m_colorEnd;

	p.m_lifeTime = props.m_lifeTime;
	p.m_lifeRemaning = props.m_lifeTime;
	p.m_sizeBegin = props.m_sizeBegin + props.m_sizeVariation * (randSt.genFloat() - 0.5f);
	p.m_sizeEnd = props.m_sizeEnd;

	if (m_poolIndex == 0)
	{
		m_poolIndex = 999;
	}
	else
	{
		--m_poolIndex;
	}
}

} //-- jny