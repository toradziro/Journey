#include "jnypch.h"
#include "ParticleSystem.h"

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
			if (!particle.m_active)
			{
				return;
			}

			if (particle.m_lifeRemaning <= 0.0f)
			{
				particle.m_active = false;
				return;
			}

			particle.m_lifeRemaning -= dt;
			particle.m_lifeRemaning -= dt;
			particle.m_position += particle.m_velocity * dt;
			particle.m_rotation += 0.01f * dt;
		});
}

void ParticleSystem::render(const OrthographicCamera& camera)
{

}

void ParticleSystem::emit(const ParticleProps& props)
{
}

} //-- jny