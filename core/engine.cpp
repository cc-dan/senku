#include "engine.h"
#include "input/input.h"

#include <iostream>
#include <thread>
#include <chrono>

void
senku::engine::start(std::function<bool(const float dt)> step)
{
	bool exit = false;
	std::chrono::steady_clock::time_point last_frame_start = std::chrono::steady_clock::now();
	while (!exit)
	{
		const std::chrono::time_point this_frame_start = std::chrono::steady_clock::now();
		const std::chrono::duration<float, std::deca> time_passed = this_frame_start - last_frame_start;
		last_frame_start = this_frame_start;

		senku::input::process(); // TODO: non-blocking
		exit = !step(time_passed.count());
		senku::engine::tick++;
	}
}
