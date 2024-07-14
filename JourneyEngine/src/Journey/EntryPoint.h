#pragma once

#ifdef JNY_PLATFORM_WINDOWS

extern jny::Application* jny::createApplication();

int main(int argc, char** argv)
{
	jny::Log::init();

	jny::Log::log(jny::Log::LogLevel::trace, "Dll main called, application init is next");

	auto app = jny::createApplication();
	app->run();
	delete app;
}

#endif
