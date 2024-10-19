#pragma once

#include "Journey/Scene/Scene.h"
#include "Panel.h"

namespace jny
{

class SceneHierarchy final : public IPanel
{
public:
	SceneHierarchy() = default;
	SceneHierarchy(const Ref<Scene>& scene);
	~SceneHierarchy();

	void setScene(const Ref<Scene>& scene) { m_scene = scene; }
	virtual void update(f32 dt) override;
	virtual void updateUI() override;

private:
	Ref<Scene>	m_scene = nullptr;
};

} //-- jny
