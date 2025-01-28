#pragma once

#include "Journey/Scene/Scene.h"
#include "Context.h"
#include "Panel.h"

namespace jny
{

class EntityProperties final : public IPanel
{
public:
	EntityProperties(const s_ptr<EditorContext>& ctx);
	~EntityProperties() = default;

	virtual void update(f32 dt) override;
	virtual void updateUI() override;
};

}