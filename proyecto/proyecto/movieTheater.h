#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include "movies.h"
#include "seats.h"
#define ROWSTHEATER 5
#define COLUMNSTHEATER 5
#define SALECLOSINGSECONDS 1800
using namespace std;
class movieTheater : public Movies {
private:
	int roomNumber;
	int price;
	int seatCount;
	time_t start;
	time_t end;
	Seats seats[ROWSTHEATER][COLUMNSTHEATER];
public:
	movieTheater(string, int, int, string, string, int, int, int, time_t, time_t);
	int getNumber() const;
	int getPrice() const;
	int getSeatCount() const;
	time_t getStart() const;
	time_t getEnd() const;
	Seats getSeat(int, int) const;
	void setRoom();
	bool fullRoom() const;
	int freeSeats() const;
	bool disponibility(int) const;
	void setReserved(int,int);
	void sold(int);
	bool hasStarted(time_t) const;
	bool canSell(time_t) const;
	bool overlaps(const movieTheater&) const;
};
