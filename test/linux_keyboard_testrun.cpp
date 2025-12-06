#include "../core/engine.h"
#include "../core/input/input.h"

#include <iostream>
#include <utility>
#include <iomanip>

std::pair<float, float> vector2;

bool
update(const float dt)
{
	vector2.first = (senku::input::is_pressed(KEY_RIGHT)? 1 : 0) - (senku::input::is_pressed(KEY_LEFT)? 1 : 0);
	vector2.second = (senku::input::is_pressed(KEY_UP)? 1 : 0) - (senku::input::is_pressed(KEY_DOWN)? 1 : 0);

	std::cout << "Vector: { " << vector2.first << ", " << vector2.second << " }" << std::endl;

	if (senku::input::is_just_pressed(KEY_RIGHT))
		std::cout << "Right is just pressed" << std::endl;
	if (senku::input::is_just_released(KEY_RIGHT))
		std::cout << "Right is released" << std::endl;

	return !senku::input::is_just_pressed(KEY_ESC);
}

int main()
{
	senku::engine::start(update);
	return 0;
}