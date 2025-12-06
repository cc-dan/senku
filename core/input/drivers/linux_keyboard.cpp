#include "linux_keyboard.h"

#include <iostream>
#include <limits>
#include <string_view>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

constexpr std::string_view devices_path = "/dev/input/by-path";

std::optional<std::filesystem::path> find_first_keyboard_device_file()
{
	for (auto &entry : std::filesystem::directory_iterator(devices_path))
	{
		if (entry.is_directory()) continue;

		const std::string as_string = entry.path().filename().string();
		if (as_string.ends_with("-event-kbd"))
			return entry.path();
	}
	return std::nullopt;
}

senku::input::drivers::linux_keyboard::linux_keyboard()
{
	const auto device_path = find_first_keyboard_device_file();
	if (not device_path)
		throw std::runtime_error("Couldn't find keyboard file");
	if ((fd = open(device_path->c_str(), O_RDONLY)) < 0)
		throw std::runtime_error("Couldn't open keyboard file: " + std::string(std::strerror(errno)));
}

std::optional<senku::input::input_event> senku::input::drivers::linux_keyboard::poll()
{
	senku::input::input_event e;

	pollfd fds[] = { { .fd = fd, .events = POLLIN } };

	const int result = ::poll(fds, 1, 0);
	if (result == -1)
		throw std::runtime_error("Poll error: " + std::string(std::strerror(errno)));
	else if (result == 0)
		return std::nullopt;

	bool got_valid_key = false;
	do
	{
		read(fd, reinterpret_cast<char*>(&event_cache), sizeof(event_cache)); // device_file.read(reinterpret_cast<char*>(&event_cache), sizeof(event_cache));

		if (event_cache.type == EV_KEY)
			if (got_valid_key = (event_cache.code <= std::numeric_limits<button_code_type>::max()))
				e = {
					.button_code = static_cast<button_code_type>(event_cache.code),
					.button_state = event_cache.value == 0? senku::input::button_states::released : senku::input::button_states::pressed
				};
	}
	while (event_cache.type != EV_SYN || !got_valid_key);

	return e;
}
