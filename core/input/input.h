#pragma once

#include <fstream>
#include <linux/input.h>

namespace senku
{
	namespace input
	{
		enum button_states
		{
			released, pressed
		};

		struct input_event
		{
			unsigned short button_code;
			button_states button_state;
		};

		class input_source
		{
		public:
			virtual input_event poll() = 0;
		};
	}
}