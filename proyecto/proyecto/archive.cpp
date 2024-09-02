#include "archive.h"
Archive::Archive() {
	this->info = "Programa hecho por Alejandro Rafael Alpizar Rodriguez para el curso de programacion.\nEl proyecto consiste en una sala de cine que funciona mediante reservas";
}
void Archive::getInfo() {
	cout << string(info);
}

