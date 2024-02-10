#pragma once
#pragma once

#include "vppch.h"
#include "Event.h"

namespace Viper {
	class JoystickConnectionEvent : public Event
	{
	public:
		JoystickConnectionEvent(bool connected) : m_Connected(connected) {}
		inline int Connected() const { return m_Connected; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "JoystickConnectionEvent: " << m_Connected;
			return ss.str();
		}
		EVENT_CLASS_TYPE(JoystickConnected);
		EVENT_CLASS_CATEGORY(EventCategoryJoystick | EventCategoryInput)
	private:
		bool m_Connected;
	};
};
