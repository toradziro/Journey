#pragma once

#include "Journey/Core/Reference.h"

namespace jny
{

class Scene;

class SceneSerializer
{
public:
	SceneSerializer() = default;
	SceneSerializer(const Ref<Scene>& scene) : m_scene(scene) {}
	~SceneSerializer() = default;

	void serialize(const std::string& filename);
	void deserialize(const std::string& filename);

	void runtimeSerialize();
	void runtimeDeserialize();

private:
	Ref<Scene>	m_scene;
};

} //-- jny