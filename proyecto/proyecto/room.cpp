#include "room.h"
Room::Room(int number, int seatCount, int price) {
	this->number = number;
	this->seatCount = seatCount;
	this->price = price;
}
int Room::getNumber() const {
	return number;
}
int Room::getSeatCount() const {
	return seatCount;
}
int Room::getPrice() const {
	return price;
}
