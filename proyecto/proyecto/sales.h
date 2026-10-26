#pragma once
#include <iostream>
#include <string>
#include "customer.h"
#define TOTALCUSTOMERS 150
#define MINIMUMIDLENGTH 9
#define MAXIMUMIDLENGTH 12
#define CARDLENGTH 16
using namespace std;
class Sales{
protected:
	Customer customer[TOTALCUSTOMERS];
public:
	bool pay(int, string, string);
	bool detectSale(int) const;
	Customer getCustomer(int) const;
	bool isValidCustomer(const string&, const string&) const;
private:
	bool hasDigits(const string&, int, int) const;
};
