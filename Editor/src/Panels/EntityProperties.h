#pragma once

#include "Journey/Scene/Scene.h"
#include "Context.h"
#include "Panel.h"

namespace jny
{

class EntityProperties final : public IPanel
{
public:
	EntityProperties(const Ref<EditorContext>& ctx);
	~EntityProperties();

	virtual void update(f32 dt) override;
	virtual void updateUI() override;
};

}