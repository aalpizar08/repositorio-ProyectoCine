#include <iostream>
#include "movieTheater.h"
#include "string.h"
#include "movies.h"
using namespace std;
movieTheater::movieTheater(string name, int year, int duration, string country, string review, int number, int price) : Movies(name, year, duration, country, review) {
	this->number = number;
	this->price = price;
	//this->schedule = schedule;
}
int movieTheater::getNumber() const {
	return number;
}

int movieTheater::getPrice() const {
	return price;
}
void movieTheater::displayAll() const {
	cout << "=========================" << endl;
	cout << "Nombre: " << name << endl;
	cout << "Anho: " << year << endl;
	cout << "Pais: " << country << endl;
	cout << "Calificacion: " << review << endl;
	cout << "Sala: " << number << endl;
	cout << "Precio: " << price << endl;
	cout << "=========================" << endl;

}