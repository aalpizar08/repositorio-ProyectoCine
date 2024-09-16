#include "seats.h"
void Seats::isReserved(){
	if (number == 0) {
		reserved = true;
	}
}
void Seats::setNumber(int number){
	this->number = number;
}
bool Seats::getReserved(){
	return reserved;
}

int Seats::getNumber(){
	return number;
}
