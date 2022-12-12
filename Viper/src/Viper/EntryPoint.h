#pragma once

#ifdef VP_PLATFORM_WINDOWS

extern Viper::Application* Viper::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Viper::CreateApplication();
	app->Run();
	delete app;
}

#endif