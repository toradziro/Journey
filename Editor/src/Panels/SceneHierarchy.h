#pragma once

#include "Journey/Scene/Scene.h"
#include "Context.h"
#include "Panel.h"

namespace jny
{

class SceneHierarchy final : public IPanel
{
public:
	SceneHierarchy(const s_ptr<EditorContext>& ctx);
	~SceneHierarchy() = default;

	virtual void update(f32 dt) override;
	virtual void updateUI() override;

private:
	void drawContextMenu();
	void resetSelection();
	void selectEntity(Entity e);
};

} //-- jny
