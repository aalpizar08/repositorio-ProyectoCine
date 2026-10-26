#pragma once
#include <cstddef>
#include <string>

struct FormField {
	std::string value;
	bool isDigitsOnly;
	std::size_t maxLength;
};
