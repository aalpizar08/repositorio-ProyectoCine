#pragma once
#include <iostream>
#include <string>
#define ROWSBOOKING 150
#define COLUMNSBOOKING 5
#define VOUCHERCOLUMN 0
#define TOTALCOLUMN 1
#define THEATERCOLUMN 2
#define TICKETSCOLUMN 3
#define PAIDCOLUMN 4
using namespace std;
class Booking {
protected:
	int vouchers[ROWSBOOKING][COLUMNSBOOKING] = {};
	int nextVoucher = 1;
public:
	int toBooking(int,int,int);
	bool detectVoucher(int) const;
	int getTheater(int) const;
	int getTickets(int) const;
	int getTotal(int) const;
	bool isPaid(int) const;
	void setPaid(int);
private:
	int findRow(int) const;
};
