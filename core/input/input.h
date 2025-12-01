#pragma once

#include <fstream>
#include <linux/input.h>
#include <array>
#include <limits>

namespace senku
{
	namespace input
	{
		using button_code_type = std::uint8_t;

		enum button_states
		{
			released, pressed
		};

		struct input_event
		{
			button_code_type button_code;
			button_states button_state;
		};

		class input_source
		{
		public:
			virtual input_event poll() = 0;
		};

		void process();
		bool is_pressed(button_code_type code);
		bool is_just_pressed(button_code_type code);
		bool is_released(button_code_type code);
		bool is_just_released(button_code_type code);
	}
}