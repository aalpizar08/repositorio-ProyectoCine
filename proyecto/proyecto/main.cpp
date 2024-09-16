#include <iostream>
#include <conio.h>
#include <ctime>
#include "archive.h"
#include "movieTheater.h"
#include "booking.h"
#include "string.h"

#define ROWS 5
#define COLUMNS 5
using namespace std;
int main() {
	string name, country, review;
	int year = 0, duration = 0;
	
	movieTheater r1(name, year, duration, country, review, 1, 4000);
	movieTheater r2("El niño y la garza", 2023, 124, "Japon", "4/5", 1, 4000);
	movieTheater r3("La vida es bella", 1997, 116, "Italia", "4,3/5", 2, 3000);
	movieTheater r4("Wallace y Gromit: la batalla de los vegetales", 2005, 85, "Reino Unido", "3,8/5", 2, 3000);
	
	r1.setRoom();
	r2.setRoom();
	r3.setRoom();
	r4.setRoom();

	Booking book;
	Sales sale;

	

	int voucher = -1;
	while (true) { //inicio del menu
		system("CLS");
		cout << "Bienvenido a la app de Nueva Cinema Perez Zeledon" << endl;
		char userSelect = {};
		cout << "Seleccione una letra segun la opcion\na.) Reserva\nb.) Compra\nc.)Mantenimiento\nd.)Archivo" << endl;
		cin >> userSelect;
		while (userSelect != 'a' && userSelect != 'b' && userSelect != 'c' && userSelect != 'd') {
			cout << "Opcion invalida, digite una letra valida\na.) Archivo\nb.) Mantenimiento\nc.)Reserva\nd.)Venta" << endl;
			cin >> userSelect;
		}
		if (userSelect == 'a') { //reserva
			int reserveSelect = 0;
			int movieSelect = 0;
			int totalTickets = 0;
			int ticketSelect = 0;
			int cont = 0;
			cout << "Seleccione una sala\n1) Sala 1\n2) Sala 2" << endl;
			cin >> reserveSelect;
			if (reserveSelect == 1) {//sala numero 1
				cout << "1)" << endl;
				r1.displayAll();//funcion 1
				cout << "2)" << endl;
				r2.displayAll();//funcion 2
				cout << "Seleccione una pelicula: " << endl;
				cin >> movieSelect;
				while (movieSelect > 3) {
					cout << "Numero invalido" << endl;
					cin >> movieSelect;
				}
				if (movieSelect == 1) {
					movieSelect--;
					if (r1.fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!r1.fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						while (cont < totalTickets) {
							r1.printSeats();
							cout << "Digite el numero de butaca que quiera reservar" << endl;
							cin >> ticketSelect;
							while (ticketSelect > 25) {
								cout << "Numero fuera de rango, elija otra butaca" << endl;
								cin >> ticketSelect;
							}
							while (!r1.disponibility(ticketSelect)) {
								cout << "Butaca reservada, elija otra" << endl;
								cin >> ticketSelect;
							}
							r1.setReserved(ticketSelect, voucher);
							cont++;
						}
						book.toBooking(voucher, r1.getPrice() * totalTickets);
					}
					voucher--;
				}
				if (movieSelect == 2) {//el niño y la garza
					if (r2.fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!r2.fullRoom()) {
						cout << "Cuantos boletos desea comprar" << endl;
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						while (cont < totalTickets) {
							r2.printSeats();
							cout << "Digite el numero de butaca que quiera reservar" << endl;
							cin >> ticketSelect;
							while (ticketSelect > 25) {
								cout << "Numero fuera de rango, elija otra butaca" << endl;
								cin >> ticketSelect;
							}
							while (!r2.disponibility(ticketSelect)) {
								cout << "Butaca reservada, elija otra" << endl;
								cin >> ticketSelect;
							}
							r2.setReserved(ticketSelect,voucher);
							cont++;
						}
						book.toBooking(voucher, r2.getPrice() * totalTickets);
						voucher--;
					}
					
				}
			}
			if (reserveSelect == 2) {//sala 2
				cout << "1)" << endl;
				r3.displayAll();//funcion 1
				cout << "2)" << endl;
				r4.displayAll();//funcion 2
				cout << "Seleccione una pelicula: " << endl;
				cin >> movieSelect;
				while (movieSelect > 3) {
					cout << "Numero invalido" << endl;
					cin >> movieSelect;
				}
				if (movieSelect == 1) {//la vida es bella
					if (r3.fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!r3.fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						while (cont < totalTickets) {
							r3.printSeats();
							cout << "Digite el numero de butaca que quiera reservar" << endl;
							cin >> ticketSelect;
							while (ticketSelect > 25) {
								cout << "Numero fuera de rango, elija otra butaca" << endl;
								cin >> ticketSelect;
							}
							while (!r3.disponibility(ticketSelect)) {
								cout << "Butaca reservada, elija otra" << endl;
								cin >> ticketSelect;
							}
							r3.setReserved(ticketSelect, voucher);
							cont++;
						}
						book.toBooking(voucher, r3.getPrice() * totalTickets);
						voucher--;
					}
				}
				if (movieSelect == 2) {//wallace y groomit
					if (r4.fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!r4.fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						while (cont < totalTickets) {
							r4.printSeats();
							cout << "Digite el numero de butaca que quiera reservar" << endl;
							cin >> ticketSelect;
							while (ticketSelect > 25) {
								cout << "Numero fuera de rango, elija otra butaca" << endl;
								cin >> ticketSelect;
							}
							while (!r4.disponibility(ticketSelect)) {
								cout << "Butaca reservada, elija otra" << endl;
								cin >> ticketSelect;
							}
							r4.setReserved(ticketSelect,voucher);
							cont++;
						}
						book.toBooking(ticketSelect, r4.getPrice() * totalTickets);
						voucher--;
					}
				}
			}
		}
		if (userSelect == 'b') {//compra
			int userVoucher = 0;
			cout << "Digite su numero de boucher " << endl;
			cin >> userVoucher;
			if (!book.detectVoucher(userVoucher)) {
				cout << "Voucher inexistente" << endl;
			}
			if (book.detectVoucher(userVoucher)) {
				sale.pay();

			}
		}
		if (userSelect == 'c') {//mantenimiento
			cout << "Aqui se puede agregar peliculas" << endl;
			system("PAUSE");
			cout << "Nombre de la pelicula" << endl;
			cin >> name;
			cout<<"Duracion de la pelicula" << endl;
			cin >> duration;
			cout << "Año de la pelicula" << endl;
			cin >> year;
			cout << "Pais de la pelicula " << endl;
			cin >> country;
			cout << "Review de la pelicula (del 1 al 5)" << endl;
			cin >> review;
		}
		if (userSelect == 'd') {//archivo
			char maintanceSelect = {};
			Archive info;
			info.getInfo();
			printf("Digite la letra 'e' para salir o cualquier otra para continuar: ");
			cin >> maintanceSelect;
			if (maintanceSelect == 'e') {
				break;
			}
		}
	}
}

