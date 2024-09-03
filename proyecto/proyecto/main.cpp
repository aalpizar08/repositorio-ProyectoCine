#include <iostream>
#include <conio.h>
#include <ctime>
#include "archive.h"
#include "movieTheater.h"
#include "movies.h"
#include "booking.h"
#include "string.h"
#include "sales.h"
using namespace std;
int main() {
	movieTheater r1("Deadpool & Wolverine", 2024, 127, "Estados Unidos", "3,6/5", 1, 4000);
	movieTheater r2("El niño y la garza", 2023, 124, "Japon", "4/5", 1, 4000);
	movieTheater r3("La vida es bella", 1997, 116, "Italia", "4,3/5", 2, 3000);
	movieTheater r4("Wallace y Gromit: la batalla de los vegetales", 2005, 85, "Reino Unido", "3,8/5", 2, 3000);
	movieTheater rooms[4] = { r1,r2, r3,r4 };

	Booking book[4];
	while (true) { //inicio del menu
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
			cout << "Seleccione una sala\n1) Sala 1\n2) Sala 2" << endl;
			cin >> reserveSelect;
			if (reserveSelect == 1) {//sala numero 1
				cout << "1)" << endl;
				rooms[0].displayAll();//funcion 1
				cout << "2)" << endl;
				rooms[1].displayAll();//funcion 2
				cout << "Seleccione una pelicula: " << endl;
				cin >> movieSelect;
				while (movieSelect > 3) {
					cout << "Numero invalido" << endl;
					cin >> movieSelect;
				}
				if (movieSelect == 1) {//deadpool
					movieSelect--;
					if (book[0].fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!book[0].fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						book[0].toBooking(totalTickets);
						book[0].createConsecutive(totalTickets, rooms[movieSelect].getNumber(), rooms[movieSelect].getPrice());
					}
				}
				if (movieSelect == 2) {//el niño y la garza
					if (book[1].fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!book[1].fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						book[1].toBooking(totalTickets);
						book[1].createConsecutive(totalTickets, rooms[1].getNumber(), rooms[1].getPrice());
					}
				}
			}
			if (reserveSelect == 2) {//sala 2
				cout << "1)" << endl;
				rooms[2].displayAll();//funcion 3
				cout << "2)" << endl;
				rooms[3].displayAll();//funcion 4
				cout << "Seleccione una pelicula: " << endl;
				cin >> movieSelect;
				while (movieSelect > 3) {
					cout << "Numero invalido" << endl;
					cin >> movieSelect;
				}
				if (movieSelect == 1) {//la vida es bella
					if (book[2].fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!book[2].fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						book[2].toBooking(totalTickets);
						book[2].createConsecutive(totalTickets, rooms[2].getNumber(), rooms[2].getPrice());
					}
				}
				if (movieSelect == 2) {//wallace y groomit
					if (book[3].fullRoom()) {
						cout << "Sala llena" << endl;
					}
					if (!book[3].fullRoom()) {
						printf("Cuantos boletos desea comprar:\n");
						cin >> totalTickets;
						while (totalTickets > 25) {
							cout << "Numero se excede, seleccione otro mas bajo" << endl;
							cin >> totalTickets;
						}
						book[3].toBooking(totalTickets);
						book[3].createConsecutive(totalTickets, rooms[3].getNumber(), rooms[3].getPrice());
					}
				}
			}
		}
		if (userSelect == 'b') {//compra
			cout << "b" << endl;
		}
		if (userSelect == 'c') {//mantenimiento



			cout << "c" << endl;
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
