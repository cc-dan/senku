#include "../input.h"

#include <array>
#include <filesystem>
#include <cerrno>
#include <cstring>

using linux_evdev_event = input_event;

namespace senku
{
	namespace input
	{
		namespace drivers
		{
			class linux_keyboard : public input_source
			{
			public:
				linux_keyboard();
				std::optional<input_event> poll() override;
			private:
				int fd;
				linux_evdev_event event_cache;
			};
		}
	}
}
