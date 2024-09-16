#include "sales.h"
void Sales::pay(){
	int ID = 0;
	int card = 0;
	cout << "Digite su numero de cedula" << endl;
	cin >> ID;
	cout << "Digite su numero de tarjeta " << endl;
	cin >> card;
	for (int i = 0; i < 25; i++) {
		if (customer[i].getID() == 0) {
			customer[i].setID(ID);
		}
		if (customer[i].getCard() == 0) {
			customer[i].setCard(card);
		}
		cout << "Pago realizado" << endl;
	}
}
bool Sales::detectVoucher(int** vouchers, int rows, int colums, int voucherNumber){
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < colums; j++) {
			if (vouchers[i][j] == voucherNumber) {
				return true;
			}
		}
	}
	return false;
}
