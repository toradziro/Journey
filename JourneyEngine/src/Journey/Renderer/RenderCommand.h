#pragma once

#include "RendererAPI.h"
#include "Journey/SingletonInterface.h"

namespace jny
{

class RenderCommand : public ISingleton
{
	JNY_SINGLETON_TYPE(RenderCommand)

public:
	void init();

	void createRenderer(RendererAPI::API api);

	void drawIndexed(const Ref<VertexArray>& vertexArray)
	{
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