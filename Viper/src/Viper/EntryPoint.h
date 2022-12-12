#pragma once

#ifdef VP_PLATFORM_WINDOWS

extern Viper::Application* Viper::CreateApplication();

int main(int argc, char** argv)
{
	Viper::Log::Init();
	VP_CORE_WARN("Initialized Logger");
	int a = 5;
	VP_INFO("Var = {0}", a);

	auto app = Viper::CreateApplication();
	app->Run();
	delete app;
}

#endif