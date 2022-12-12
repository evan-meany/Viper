#pragma once

#include "Core.h"

namespace Viper
{
	class VIPER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();

}
