#pragma once
#include <iostream>
#include "string.h"
#include "movies.h"
#include "seats.h"
#define ROWSTHEATER 5
#define COLUMNSTHEATER 5
using namespace std;
class movieTheater : public Movies {
private:
	int roomNumber;
	int price;
	Seats seats[ROWSTHEATER][COLUMNSTHEATER];
public:
	movieTheater(string, int, int, string, string, int, int);
	int getNumber() const;
	int getPrice() const;
	void displayAll() const;
	void setRoom();
	void printSeats();
	bool fullRoom();
	bool disponibility(int);
	void setReserved(int,int);
	void sold(int,int);
};