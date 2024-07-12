#pragma once

#ifdef JNY_PLATFORM_WINDOWS

extern jny::Application* jny::createApplication();

int main(int argc, char** argv)
{
	printf("Journey starting point here\n");
	auto app = jny::createApplication();
	app->run();
	delete app;
}

#endif
