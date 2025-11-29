#include "../core/input/drivers/linux_keyboard.h"

#include <iostream>

int main()
{
	senku::input::drivers::linux_keyboard kb;

	while (true)
	{
		const senku::input::input_event e = kb.poll();

		std::cout << "Key code " << e.button_code << (e.button_state == senku::input::button_states::released? " released" : " pressed") << std::endl;
	}
}