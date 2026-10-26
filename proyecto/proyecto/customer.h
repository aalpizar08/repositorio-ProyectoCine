#pragma once
#include <string>
using namespace std;
class Customer
{
private:
	int voucher = 0;
	string id;
	string card;
public:
	int getVoucher() const;
	string getID() const;
	string getCard() const;
	string getMaskedCard() const;
	void setVoucher(int);
	void setID(string);
	void setCard(string);
};
