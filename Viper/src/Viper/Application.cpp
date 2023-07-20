#include "Application.h"

#include "Events/Event.h"
#include "Viper/Events/KeyEvent.h"
#include "Viper/Log.h"

namespace Viper
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		KeyPressedEvent e(7, 0);
		VP_TRACE(e);

		while (true);
	}
}