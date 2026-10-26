#include "dateTime.h"
#include <sstream>

bool DateTime::parse(const std::string& date, const std::string& time, std::time_t& result) {
	int day = 0;
	int month = 0;
	int year = 0;
	int hour = -1;
	int minute = -1;
	char dateSeparator = ' ';
	char secondDateSeparator = ' ';
	char timeSeparator = ' ';
	std::istringstream dateStream(date);
	std::istringstream timeStream(time);
	dateStream >> day >> dateSeparator >> month >> secondDateSeparator >> year;
	timeStream >> hour >> timeSeparator >> minute;
	bool hasDateFormat = !dateStream.fail() && dateStream.eof() && dateSeparator == '/' && secondDateSeparator == '/';
	bool hasTimeFormat = !timeStream.fail() && timeStream.eof() && timeSeparator == ':';
	if (!hasDateFormat || !hasTimeFormat || hour > 23 || minute > 59 || hour < 0 || minute < 0) {
		return false;
	}
	std::tm parts = {};
	parts.tm_mday = day;
	parts.tm_mon = month - 1;
	parts.tm_year = year - 1900;
	parts.tm_hour = hour;
	parts.tm_min = minute;
	parts.tm_isdst = -1;
	result = std::mktime(&parts);
	return result != -1 && parts.tm_mday == day && parts.tm_mon == month - 1 && parts.tm_year == year - 1900;
}
std::string DateTime::formatDate(std::time_t value) {
	return format(value, "%d/%m/%Y");
}
std::string DateTime::formatTime(std::time_t value) {
	return format(value, "%H:%M");
}
std::string DateTime::format(std::time_t value, const char* pattern) {
	std::tm parts = {};
	localtime_s(&parts, &value);
	char buffer[32] = {};
	std::strftime(buffer, sizeof(buffer), pattern, &parts);
	return buffer;
}
