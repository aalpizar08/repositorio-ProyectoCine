#include <iostream>
#include "movieTheater.h"
#include "movies.h"
using namespace std;
movieTheater::movieTheater(string name, int year, int duration, string country, string review, int roomNumber, int price, int seatCount, time_t start, time_t end) : Movies(name, year, duration, country, review) {
	this->roomNumber = roomNumber;
	this->price = price;
	this->seatCount = seatCount;
	this->start = start;
	this->end = end;
	setRoom();
}
int movieTheater::getNumber() const {
	return roomNumber;
}

int movieTheater::getPrice() const {
	return price;
}
int movieTheater::getSeatCount() const {
	return seatCount;
}
time_t movieTheater::getStart() const {
	return start;
}
time_t movieTheater::getEnd() const {
	return end;
}
Seats movieTheater::getSeat(int row, int column) const {
	return seats[row][column];
}
void movieTheater::setRoom(){
	int numberSeat = 1;
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (numberSeat <= seatCount) {
				seats[i][j].enable();
				seats[i][j].setNumber(numberSeat);
			}
			numberSeat++;
		}
	}
}
bool movieTheater::fullRoom() const {
	return freeSeats() == 0;
}
int movieTheater::freeSeats() const {
	int cont = 0;
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].isAvailable()) {
				cont++;
			}
		}
	}
	return cont;
}
bool movieTheater::disponibility(int ticket) const {
	if (ticket <= 0) {
		return false;
	}
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].getNumber() == ticket) {
				return true;
			}
		}
	}
	return false;
}
void movieTheater::setReserved(int numberSeat,int voucher){
	if (!disponibility(numberSeat)) {
		return;
	}
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].getNumber() == numberSeat) {
				seats[i][j].setNumber(-voucher);
			}
		}
	}
}
void movieTheater::sold(int voucher) {
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].isReserved() && seats[i][j].getNumber() == -voucher) {
				seats[i][j].setNumber(0);
			}
		}
	}
}
bool movieTheater::hasStarted(time_t now) const {
	return now >= start;
}
bool movieTheater::canSell(time_t now) const {
	return now < start - SALECLOSINGSECONDS;
}
bool movieTheater::overlaps(const movieTheater& other) const {
	return roomNumber == other.roomNumber && start < other.end && other.start < end;
}
