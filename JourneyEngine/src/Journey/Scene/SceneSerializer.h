#pragma once

#include "Journey/Core/Reference.h"

namespace YAML
{

class Emitter;

}

namespace jny
{

class Scene;
class Entity;

class SceneSerializer
{
public:
	SceneSerializer() = default;
	SceneSerializer(Scene* scene) : m_scene(scene) {}
	~SceneSerializer() = default;

	void serialize(const std::string& filename);
	void deserialize(const std::string& filename);

	void runtimeSerialize();
	void runtimeDeserialize();

private:
	void serializeEntity(YAML::Emitter& sFile, Entity e);

	Scene*	m_scene;
};

} //-- jny