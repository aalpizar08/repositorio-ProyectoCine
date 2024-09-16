#include "customer.h"

int Customer::getID(){
	return id;
}
int Customer::getCard(){
	return card;
}
void Customer::setID(int id){
	this->id = id;
}
void Customer::setCard(int card){
	this->card = card;
}
