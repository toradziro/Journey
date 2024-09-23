#pragma once

#include <glm/glm.hpp>
#include "Journey/Renderer/OrthographicCamera.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Core/Random.h"

namespace jny
{

struct ParticleProps
{
	glm::vec3	m_position = {};
	glm::vec3	m_velocity = {};
	glm::vec2	m_velocityVariation = {};
	glm::vec4	m_colorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4	m_colorEnd = { 1.0f, 1.0f, 1.0f, 1.0f };
	float		m_sizeBegin = 0.0f;
	float		m_sizeEnd = 0.0f;
	float		m_sizeVariation = 0.0f;
	float		m_lifeTime = 0.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void update(float dt);
	void render();

	void emit(const ParticleProps& props);

private:
	struct Particle
	{
		glm::vec3	m_position = {};
		glm::vec3	m_velocity = {};
		glm::vec2	m_size = {};
		glm::vec4	m_colorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4	m_colorEnd = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float		m_rotation = 0.0f;
		float		m_sizeBegin = 0.0f;
		float		m_sizeEnd = 0.0f;
		float		m_lifeTime = 0.0f;
		float		m_lifeRemaning = 0.0f;
		bool		m_active = false;
	};

	std::vector<Particle>	m_particlePool;
	uint32_t				m_poolIndex = 999;

	uint32_t				m_quadVA = 0;
	Ref<Shader>				m_particleShader = nullptr;
	int32_t					m_particleShaderViewProj = 0;
	int32_t					m_particleShaderTransform = 0;
	int32_t					m_particleShaderColor = 0;
};

} //-- jny