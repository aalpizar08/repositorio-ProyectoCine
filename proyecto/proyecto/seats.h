#pragma once
#include <iostream>
#include <string>
#include "sales.h"
using namespace std;
class Seats{
protected:
	int number = 0;
	bool reserved = false;
public:
	void isReserved();
	void setNumber(int);

	bool getReserved();
	int getNumber();
};

