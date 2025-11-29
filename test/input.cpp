#include <fstream>
#include <iostream>
#include <linux/input.h> // /usr/include/linux/input.h
#include <array>
#include <filesystem>
#include <optional>
#include <cerrno>
#include <cstring>
#include <string>
#include <stdexcept>
#include <utility>

// https://docs.kernel.org/input/event-codes.html

const std::filesystem::path devices_directory("/dev/input/by-path/");
enum key_state_types { released, pressed, held };
unsigned tick;

const std::optional<std::filesystem::path> get_keyboard_device_path()
{
	for (auto &directory_entry : std::filesystem::directory_iterator(devices_directory))
	{
		const std::string as_string = directory_entry.path().filename().string();
		if (as_string.ends_with("-event-kbd") && !directory_entry.is_directory())
			return directory_entry.path();
	}
	return std::nullopt;
}

class key_state
{
public:
	key_state() : state(key_state_types::released) {}

	void set_state(key_state_types s)
	{
		previous_state = state;
		state = s;
		last_update = tick;
	}

	bool is_pressed() const
	{
		return state == key_state_types::pressed || state == key_state_types::held;
	}

	bool is_just_pressed() const
	{
		return is_pressed() && previous_state == key_state_types::released;
	}

	bool is_released() const
	{
		return state == key_state_types::released;
	}

	bool is_just_released() const
	{
		return is_released() &&
		       (previous_state == key_state_types::pressed || previous_state == key_state_types::held) &&
		       last_update == tick; 
	}
private:
	key_state_types state;
	key_state_types previous_state;
	unsigned last_update;
};

std::array<key_state, 256> key_states;
struct input_event input_event;

void poll_keyboard(std::ifstream &device)
{
	do
	{
		device.read(reinterpret_cast<char*>(&input_event), sizeof(input_event));
std::clog << input_event.type << " " << input_event.value << std::endl;
		const bool keycode_in_range = input_event.code < key_states.size();
		if (input_event.type == EV_KEY && keycode_in_range)
			key_states[input_event.code].set_state(static_cast<key_state_types>(input_event.value));
	}
	while (input_event.type != EV_SYN);

	// while (true)
	// {
	// 	device.read(reinterpret_cast<char*>(&input_event), sizeof(input_event));
	// 	std::clog << input_event.type << " " << input_event.value << std::endl;
	// }
}

bool is_pressed(unsigned button_code)
{
	if (button_code > key_states.size())
		return false;

	return key_states[button_code].is_pressed();
}

bool is_just_pressed(unsigned button_code)
{
	if (button_code > key_states.size())
		return false;

	return key_states[button_code].is_just_pressed();
}

bool is_just_released(unsigned button_code)
{
	if (button_code > key_states.size())
		return false;

	return key_states[button_code].is_just_released();
}

int main()
{
	const auto path = get_keyboard_device_path();
	if (not path)
	{
		std::cerr << "Couldn't find a keyboard" << std::endl;
		return 1;
	}
	std::cout << "Keyboard found at " << path->c_str() << std::endl;

	std::ifstream device(*path, std::ios::binary | std::ios::in);
	if (not device.good())
	{
		std::cerr << "Error opening input device for reading: " << std::strerror(errno) << std::endl;
		return 2;
	}

	std::pair<float, float> vector2;
	while (true)
	{
		poll_keyboard(device);

		if (is_just_pressed(KEY_RIGHT))
			std::cout << "Just pressed right" << std::endl;
		if (is_just_released(KEY_RIGHT))
			std::cout << "Released right" << std::endl;

		vector2.first = (is_pressed(KEY_RIGHT)? 1 : 0) - (is_pressed(KEY_LEFT)? 1 : 0);
		vector2.second = (is_pressed(KEY_UP)? 1 : 0) - (is_pressed(KEY_DOWN)? 1 : 0);

		std::cout << "Vector: { " << vector2.first << ", " << vector2.second << " }" << std::endl;

		tick++;
	}
}
