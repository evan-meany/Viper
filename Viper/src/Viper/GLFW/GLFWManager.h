#pragma once

namespace Viper {

	// This class should manage the GLFW instance and poll for events
	class GLFWManager
	{
	public:
		static void Init();
		static void OnUpdate();
		// TODO: change this to Unique probably
		static Shared<GLFWManager> Get() { return s_Instance; }
	private:
		static Shared<GLFWManager> s_Instance;
	};

}