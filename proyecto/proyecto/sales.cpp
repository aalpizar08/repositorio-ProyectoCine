#include "sales.h"
bool Sales::pay(int voucher, string ID, string card){
	if (voucher <= 0 || detectSale(voucher) || !isValidCustomer(ID, card)) {
		return false;
	}
	for (int i = 0; i < TOTALCUSTOMERS; i++) {
		if (customer[i].getVoucher() == 0) {
			customer[i].setVoucher(voucher);
			customer[i].setID(ID);
			customer[i].setCard(card);
			return true;
		}
	}
	return false;
}
bool Sales::detectSale(int voucher) const{
	for (int i = 0; i < TOTALCUSTOMERS; i++) {
		if (voucher > 0 && customer[i].getVoucher() == voucher) {
			return true;
		}
	}
	return false;
}
Customer Sales::getCustomer(int voucher) const{
	for (int i = 0; i < TOTALCUSTOMERS; i++) {
		if (customer[i].getVoucher() == voucher) {
			return customer[i];
		}
	}
	return Customer();
}
bool Sales::isValidCustomer(const string& ID, const string& card) const{
	return hasDigits(ID, MINIMUMIDLENGTH, MAXIMUMIDLENGTH) && hasDigits(card, CARDLENGTH, CARDLENGTH);
}
bool Sales::hasDigits(const string& value, int minimumLength, int maximumLength) const{
	int length = (int)value.size();
	if (length < minimumLength || length > maximumLength) {
		return false;
	}
	for (char character : value) {
		if (character < '0' || character > '9') {
			return false;
		}
	}
	return true;
}
