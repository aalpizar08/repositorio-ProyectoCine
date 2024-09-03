#pragma once
#include <iostream>
#include "string.h"
using namespace std;
class Sales{
protected:
	int ID;
	int cardNumber;
public:
	Sales(int, int);
	int getID() const;
	int getCardNumber() const;
	void pay();

};

