#pragma once
#include <iostream>
#include "string.h"
using namespace std;
class Movies {
protected:
	string name;
	int year;
	int duration;
	string country;
	string review;
	bool isMovie = false;
public:
	Movies(string, int, int, string, string);
	~Movies();
	string getName();
	int getYear() const;
	int getDuration() const;
	string getCountry()const;
	string getReview()const;
	bool getIsMovie();
};