#pragma once

//-- To provide API for applications

#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/InputPoll.h"
#include "Journey/Window/Window.h"
#include "Journey/Log/Log.h"

#include "Journey/Events/Event.h"
#include "Journey/Events/ApplicationEvent.h"
#include "Journey/Events/KeyEvent.h"
#include "Journey/Events/MouseEvent.h"

#include "Journey/Layer/Layer.h"

#include "Journey/ImGui/ImGuiLayer.h"

#include "Journey/Renderer/Buffer.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Renderer/RenderCommand.h"
#include "Journey/Renderer/OrthographicCamera.h"
#include "Journey/Renderer/Texture.h"
#include "Journey/Renderer/OrthographicCameraController.h"
#include "Journey/Renderer/Framebuffer.h"

#include "Journey/Scene/Scene.h"
#include "Journey/Scene/Entity.h"
#include "Journey/Scene/Components.h"

#include "Journey/Core/Profiling/TimeInstruments.h"
#include "Journey/Core/Profiling/Utils.h"
#include "Journey/Core/ImGuiIntegration.h"
#include "Journey/Core/fs/VirtualFileSystem.h"