#pragma once

#include "jnypch.h"
#include "Journey/Scene/Scene.h"
#include "Journey/Scene/Entity.h"
#include "Journey/Core/Reference.h"

namespace jny
{

struct EditorContext : public ReferenceCounter
{
	Ref<Scene>	m_currentScene;
	Entity		m_selectedEntity;
};

} //-- jny