#include "customer.h"

int Customer::getVoucher() const{
	return voucher;
}
string Customer::getID() const{
	return id;
}
string Customer::getCard() const{
	return card;
}
string Customer::getMaskedCard() const{
	if (card.size() < 4) {
		return card;
	}
	return "**** **** **** " + card.substr(card.size() - 4);
}
void Customer::setVoucher(int voucher){
	this->voucher = voucher;
}
void Customer::setID(string id){
	this->id = id;
}
void Customer::setCard(string card){
	this->card = card;
}
