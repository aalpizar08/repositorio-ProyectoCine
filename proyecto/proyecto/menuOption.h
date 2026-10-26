#pragma once
#include <functional>
#include <string>

struct MenuOption {
	std::string label;
	std::function<void()> action;
};
