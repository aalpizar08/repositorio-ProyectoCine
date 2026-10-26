#pragma once
#include <iostream>
using namespace std;
class Room {
private:
	int number;
	int seatCount;
	int price;
public:
	Room(int, int, int);
	int getNumber() const;
	int getSeatCount() const;
	int getPrice() const;
};
