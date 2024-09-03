#include "sales.h"
Sales::Sales(int ID, int cardNumber){
	this->ID = ID;
	this->cardNumber = cardNumber;
}
int Sales::getID() const{
	return ID;
}
int Sales::getCardNumber() const{
	return cardNumber;
}
void Sales::pay(){

}
