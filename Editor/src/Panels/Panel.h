#pragma once

#include "jnypch.h"
#include "Context.h"
#include "Journey/Core/Reference.h"

namespace jny
{

class IPanel
{
public:
	IPanel(const Ref<EditorContext>& context) : m_ctx(context) {}

	virtual void update(f32 dt) = 0;
	virtual void updateUI() = 0;

	Ref<EditorContext>& ctx() { return m_ctx; }

protected:
	Ref<EditorContext>	m_ctx;
};

} //-- jny