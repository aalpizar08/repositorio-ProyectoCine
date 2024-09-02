#pragma once
#include <iostream>
#include <ctime>
#include "string.h"
#include "movies.h"
using namespace std;
class movieTheater : public Movies {
private:
	int number;
	int seats[5][5] = { { 1,2,3,4,5},{ 6,7,8,9,10},{ 11,12,13,14,15},{ 16,17,18,19,20},{ 21,22,23,24,25} };
	int price;
	tm schedule = {};
public:
	movieTheater(string, int, int, string, string, int, int);
	int getNumber() const;
	int getPrice() const;
	void displayAll() const;
};