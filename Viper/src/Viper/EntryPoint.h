#pragma once

#ifdef VP_PLATFORM_WINDOWS

extern Viper::Application* Viper::CreateApplication();

int main(int argc, char** argv)
{
	Viper::Log::Init();
	VP_CORE_WARN("Initialized Logger");

	Viper::Application* app = Viper::CreateApplication();
	app->Run();
	delete app;
}

#endif