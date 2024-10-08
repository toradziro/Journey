#pragma once

extern jny::Application* jny::createApplication();

int main(int /*argc*/, char** /*argv*/)
{
	jny::Log::init();

	jny::Log::trace("Dll main called: {} {} {}", __FILE__, __LINE__, __FUNCTION__);

	auto app = jny::createApplication();
	app->run();
	delete app;
}
