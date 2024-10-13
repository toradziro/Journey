#include "jnypch.h"
#include "Editor.h"
#include "EditorLayer.h"

#include "Journey/Core/InputPoll.h"
#include "Journey/Core/EntryPoint.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace jny
{

Editor::Editor() :
	Application("Editor")
{
	//disableImGui();
	this->pushLayer<EditorLayer>(static_cast<Application*>(this));
}

Editor::~Editor() {}

Application* createApplication()
{
	return new Editor();
}

}