#include <iostream>
#include "string.h"
#include "seats.h"
#define ROWSBOOKING 150
#define COLUMNSBOOKING 2
using namespace std;
class Booking {
protected:
	int vouchers[ROWSBOOKING][COLUMNSBOOKING];
public:
	void toBooking(int,int);
	bool detectVoucher(int);

};
