#pragma once

#include "jnypch.h"
#include "Journey/Scene/Scene.h"
#include "Journey/Scene/Entity.h"
#include "Journey/Core/Reference.h"
#include "Journey/Renderer/EditorCamera.h"

namespace jny
{

struct EditorContext : public ReferenceCounter
{
	Ref<Scene>		m_currentScene;
	EditorCamera	m_editorCamera;
	Entity			m_selectedEntity;

	bool			m_sceneChanged = false;
};

} //-- jny