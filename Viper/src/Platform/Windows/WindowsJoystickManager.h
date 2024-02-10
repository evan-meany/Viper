#pragma once

#include "Viper/Input/JoystickManager.h"
#include "Viper/Events/Event.h"

namespace Viper {

	class WindowsJoystickManager : public JoystickManager
	{
	public:
		WindowsJoystickManager();
		~WindowsJoystickManager();
	protected:
		virtual void SetEventCallbackImpl(const EventCallbackFn& callback) override
		{
			s_Data->EventCallback = callback;
		}
		virtual int GetPrimaryPlayerIDImpl() override;

	private:
		struct ControllerData
		{
			std::vector<int> ConnectedIDs; // Note the order corresponds to the Player number
			EventCallbackFn EventCallback;
		};
		static Unique<ControllerData> s_Data;
	};

}