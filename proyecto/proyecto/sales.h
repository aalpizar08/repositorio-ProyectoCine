#pragma once
#include <iostream>
#include "string.h"
#include "customer.h"
using namespace std;
class Sales{
protected:
	Customer customer[25];
public:
	void pay();
	bool detectVoucher(int**,int,int,int);
};

