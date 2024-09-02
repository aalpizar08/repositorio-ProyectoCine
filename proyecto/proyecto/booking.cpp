#include "booking.h"
void Booking::toBooking(int tickets) {
	int contador = 0;
	while (contador < tickets) {
		int selectSeat = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cout << viewUser[i][j] << ' ';
			}
			cout << endl;
		}
		cout << "Seleccione un asiento: " << endl;
		cin >> selectSeat;
		while (selectSeat > 25) {
			cout << "Numero fuera de rango, seleccione otro" << endl;
			cin >> selectSeat;
		}
		bool detectD = disponibility(selectSeat);
		while (!detectD) {
			cout << "Asiento ocupado, seleccione otro" << endl;
			cin >> selectSeat;
			detectD = disponibility(selectSeat);
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (selectSeat == seats[i][j]) {
					seats[i][j] = 0;
					viewUser[i][j] = "X";
					cout << "Reserva realizada" << endl;
					contador++;
				}
			}
		}
	}
}
void Booking::createConsecutive(int totalTicket, int roomNumber, int pay) {
	string number1 = to_string(totalTicket);
	string number2 = to_string(roomNumber);
	string number = number1 + number2;
	int boucher = stoi(number);
	for (int i = 0; i < 25; i++) {
		if (consecutive[i][0] == 0) {
			consecutive[i][0] = boucher;
			consecutive[i][1] ;
		}
	}
}
bool Booking::disponibility(int numberSeat) const {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (numberSeat == seats[i][j]) {
				return true;
			}
		}
	}
	return false;
}
bool Booking::fullRoom()const {
	int totalClients = 0;
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			if (seats[i][j] == 0);
			totalClients++;
		}
	}
	if (totalClients == 25) {
		return true;
	}
	return false;
}
