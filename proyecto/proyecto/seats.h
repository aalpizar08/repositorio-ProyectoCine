#pragma once
#include <iostream>
#include <string>
using namespace std;
class Seats{
protected:
	int number = 0;
	bool enabled = false;
public:
	void setNumber(int);
	void enable();
	int getNumber() const;
	bool isEnabled() const;
	bool isAvailable() const;
	bool isReserved() const;
	bool isSold() const;
};
