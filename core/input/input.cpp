#include "../engine.h"
#include "input.h"
#include "drivers/linux_keyboard.h"

#include <memory>

struct button_state
{
	button_state() : current_state(senku::input::button_states::released),
	                 previous_state(senku::input::button_states::released) {}

	void set_state(senku::input::button_states s)
	{
		previous_state = current_state;
		current_state = s;
		timestamp = senku::engine::tick;
	}

	senku::input::button_states current_state, previous_state;
	unsigned timestamp;
};

std::unique_ptr<senku::input::input_source> input_event_source = std::make_unique<senku::input::drivers::linux_keyboard>();
std::array<button_state, std::numeric_limits<senku::input::button_code_type>::max() + 1> state_per_button_code;

void
senku::input::process()
{
	const senku::input::input_event e = input_event_source->poll();
	state_per_button_code[e.button_code].set_state(e.button_state);
}

bool
senku::input::is_pressed(senku::input::button_code_type code)
{
	return state_per_button_code[code].current_state == senku::input::button_states::pressed;
}

bool
senku::input::is_just_pressed(senku::input::button_code_type code)
{
	return senku::input::is_pressed(code) && state_per_button_code[code].previous_state == senku::input::button_states::released;
}

bool
senku::input::is_released(senku::input::button_code_type code)
{
	return state_per_button_code[code].current_state == senku::input::released;
}

bool
senku::input::is_just_released(senku::input::button_code_type code)
{
	return senku::input::is_released(code) &&
	       state_per_button_code[code].previous_state == senku::input::pressed &&
	       state_per_button_code[code].timestamp == senku::engine::tick;
}