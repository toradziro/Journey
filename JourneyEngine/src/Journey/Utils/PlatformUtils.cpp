#include "jnypch.h"
#include "PlatformUtils.h"
#include "Journey/Core/Application.h"
#include "Journey/Window/Window.h"

#ifdef JNY_PLATFORM_WINDOWS
#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
#include <GLFW/glfw3.h>

namespace
{
#ifdef JNY_PLATFORM_WINDOWS
std::string windowsOpenFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;			//-- Structure for the open file dialog
	CHAR file[260] = { 0 };		//-- Buffer for the file name
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = glfwGetWin32Window(jny::Application::subsystems().st<jny::Window>().rawWindow());	//-- parent HWND
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = file;
	ofn.nMaxFile = sizeof(file);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) != FALSE)	//-- Call the dialog
	{
		return ofn.lpstrFile;	//-- Store the result
	}
	return {};
}

std::string windowsSaveFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;			//-- Structure for the save file dialog
	CHAR file[260] = { 0 };		//-- Buffer for the file name
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = glfwGetWin32Window(jny::Application::subsystems().st<jny::Window>().rawWindow());	//-- parent HWND
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = file;
	ofn.nMaxFile = sizeof(file);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn) != FALSE)	//-- Call the dialog
	{
		return ofn.lpstrFile;	//-- Store the result
	}
	return {};
}
#endif
}

namespace jny::fs
{

std::string openFileDialog(const char* filter)
{
	std::string res;
#ifdef JNY_PLATFORM_WINDOWS
	res = windowsOpenFileDialog(filter);
#endif
	return res;
}

std::string saveFileDialog(const char* filter)
{
	std::string res;
#ifdef JNY_PLATFORM_WINDOWS
	res = windowsSaveFileDialog(filter);
#endif
	return res;
}

}