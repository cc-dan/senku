#pragma once

#include <functional>

namespace senku
{
	namespace engine
	{
		inline unsigned tick;

		void start(std::function<bool(const float dt)> step);
	}
}
