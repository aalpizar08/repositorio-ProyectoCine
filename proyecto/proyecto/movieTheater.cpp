#include <iostream>
#include "movieTheater.h"
#include "string.h"
#include "movies.h"
using namespace std;
movieTheater::movieTheater(string name, int year, int duration, string country, string review, int roomNumber, int price) : Movies(name, year, duration, country, review) {
	this->roomNumber = roomNumber;
	this->price = price;
	//this->schedule = schedule;
}
int movieTheater::getNumber() const {
	return roomNumber;
}

int movieTheater::getPrice() const {
	return price;
}
void movieTheater::displayAll() const {
	cout << "=========================" << endl;
	cout << "Nombre: " << name << endl;
	cout << "Año: " << year << endl;
	cout << "Pais: " << country << endl;
	cout << "Calificacion: " << review << endl;
	cout << "Sala: " << roomNumber << endl;
	cout << "Precio: " << price << endl;
	cout << "=========================" << endl;

}
void movieTheater::setRoom(){
	int numberSeat = 1;
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			seats[i][j].setNumber(numberSeat);
			numberSeat++;
		}
	}
}
void movieTheater::printSeats(){
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].getNumber() > 0) {
				cout << "{ " << seats[i][j].getNumber() << " }" << " ";
			}
			if (seats[i][j].getNumber() < 0) {
				cout << "{ " << "R" << " }";//para butacas reservadas
			}
			if (seats[i][j].getNumber() == 0) {
				cout << "{ " << "V" << " }"; //butacas vendidas
			}

		}
		cout << endl;
	}
	cout << endl;
}
bool movieTheater::fullRoom(){
	int cont = 0;
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].getNumber() == 0) {
				cont++;
			}
		}
	}
	if (cont == 25) {
		return true;
	}
	return false;
}
bool movieTheater::disponibility(int ticket) {
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
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (seats[i][j].getNumber() == numberSeat) {
				seats[i][j].setNumber(voucher);
				seats[i][j].isReserved();
			}
		}
	}
}
void movieTheater::sold(int numberSeat,int voucher) {
	int zero = 0;
	for (int i = 0; i < ROWSTHEATER; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			if (voucher == seats[i][j].getNumber()) {
				seats[i][j].setNumber(zero);
			}
		}
	}
}
