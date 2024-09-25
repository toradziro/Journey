#pragma once

#include "RendererAPI.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Core/Profiling/TimeInstruments.h"

namespace jny
{

class RenderCommand : public ISingleton
{
	JNY_SINGLETON_TYPE(RenderCommand)

public:
	void init();

	void windowResized(uint32_t width, uint32_t height);

	void createRenderer(RendererAPI::API api);

	void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t size = 0)
	{
		PROFILE_FUNC;

		m_RendererAPI->drawIndexed(vertexArray);
	}

	void setClearColor(const glm::vec4& color)
	{
		m_RendererAPI->setClearColor(color);
	}

	void clear()
	{
		m_RendererAPI->clear();
	}

	RendererAPI::API api() const { return m_RendererAPI->api(); }

private:
	std::unique_ptr<RendererAPI> m_RendererAPI;
};

} //-- jny