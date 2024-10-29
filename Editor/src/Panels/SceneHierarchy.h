#pragma once

#include "Journey/Scene/Scene.h"
#include "Context.h"
#include "Panel.h"

namespace jny
{

class SceneHierarchy final : public IPanel
{
public:
	SceneHierarchy(const Ref<EditorContext>& ctx);
	~SceneHierarchy();

	virtual void update(f32 dt) override;
	virtual void updateUI() override;

private:
	void drawContextMenu();
};

} //-- jny
