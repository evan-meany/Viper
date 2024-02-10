#include "vppch.h"
#include "WindowsInput.h"
#include "Viper/Application/Application.h"

#include <GLFW/glfw3.h>

namespace Viper {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [xpos, ypos] = GetMousePosImpl();
		return (float)xpos;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [xpos, ypos] = GetMousePosImpl();
		return (float)ypos;
	}

	void WindowsInput::HideCursorImpl(bool hide)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		if (hide)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	JoystickData WindowsInput::GetJoystickDataImpl(int joystickID)
	{
		JoystickData joystickData;
		int count;
		const float* axes = glfwGetJoystickAxes(joystickID, &count);
		if (count == 0) { VP_CORE_ASSERT("No data for joystick {0}", joystickID); }
		joystickData.LeftStickX = axes[0];
		joystickData.LeftStickY = axes[1];
		joystickData.RightStickX = axes[2];
		joystickData.RightStickY = axes[3];
		return joystickData;
	}

} // End namespace Viper