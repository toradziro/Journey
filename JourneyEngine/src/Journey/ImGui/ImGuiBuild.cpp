#include "jnypch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#ifdef JNY_PLATFORM_WINDOWS

#define sscanf sscanf_s
#define strcpy strcpy_s
#define strcat strcat_s

#endif

#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"