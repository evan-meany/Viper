#pragma once

#include "Viper/Events/Event.h"

namespace Viper {

	class JoystickManager {
	public:
		static void SetEventCallback(const EventCallbackFn& callback) { s_Instance->SetEventCallbackImpl(callback); }
		static int GetPrimaryPlayerID() { return s_Instance->GetPrimaryPlayerIDImpl(); }
		static void Create();
	protected:
		virtual void SetEventCallbackImpl(const EventCallbackFn& callback) = 0;
		virtual int GetPrimaryPlayerIDImpl() = 0;
	private:
		static Unique<JoystickManager> s_Instance;
	};

}