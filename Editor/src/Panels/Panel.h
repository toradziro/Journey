#pragma once

#include "jnypch.h"
#include "Context.h"
#include "Journey/Core/Reference.h"

namespace jny
{

class IPanel
{
public:
	IPanel(const s_ptr<EditorContext>& context) : m_ctx(context) {}

	virtual void update(f32 dt) = 0;
	virtual void updateUI() = 0;

	s_ptr<EditorContext>& ctx() { return m_ctx; }

protected:
	s_ptr<EditorContext>	m_ctx;
};

} //-- jny