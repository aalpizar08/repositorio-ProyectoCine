#include "seats.h"
void Seats::setNumber(int number){
	this->number = number;
}
void Seats::enable(){
	enabled = true;
}
int Seats::getNumber() const{
	return number;
}
bool Seats::isEnabled() const{
	return enabled;
}
bool Seats::isAvailable() const{
	return enabled && number > 0;
}
bool Seats::isReserved() const{
	return enabled && number < 0;
}
bool Seats::isSold() const{
	return enabled && number == 0;
}
