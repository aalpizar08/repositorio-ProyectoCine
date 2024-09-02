#include "movies.h"
Movies::Movies(string name, int year, int duration, string country, string review) {
	this->name = name;
	this->year = year;
	this->duration = duration;
	this->country = country;
	this->review = review;
}
Movies::~Movies() {}
string Movies::getName() {
	return this->name;
}
int Movies::getYear() const {
	return this->year;
}
int Movies::getDuration() const {
	return this->duration;
}
string Movies::getCountry() const {
	return this->country;
}
string Movies::getReview() const {
	return this->review;
}