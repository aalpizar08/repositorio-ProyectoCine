#pragma once
#include <ctime>
#include <string>

class DateTime {
public:
	static bool parse(const std::string& date, const std::string& time, std::time_t& result);
	static std::string formatDate(std::time_t value);
	static std::string formatTime(std::time_t value);
private:
	static std::string format(std::time_t value, const char* pattern);
};
