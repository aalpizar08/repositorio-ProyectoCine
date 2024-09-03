#include <iostream>
#include "string.h"
#include "string"
using namespace std;
class Booking {
protected:
	int consecutive[25];
	int seats[5][5] = { { 1,2,3,4,5},
						{ 6,7,8,9,10},
						{ 11,12,13,14,15},
						{ 16,17,18,19,20},
						{ 21,22,23,24,25} };
	string viewUser[5][5] = { { "'1'","'2'","'3'","'4'","'5'"},
						{ "'6'","'7'","'8'","'9'","'10'"},
						{ "'11'","'12'","'13'","'14'","'15'"},
						{ "'16'","'17'","'18'","'19'","'20'"},
						{ "'21'","'22'","'23'","'24'","'25'"} };
public:
	void toBooking(int);
	void createConsecutive(int, int, int);
	bool disponibility(int)const;
	bool fullRoom()const;
};
