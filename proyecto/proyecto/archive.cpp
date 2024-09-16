#include "archive.h"
Archive::Archive() {
	this->info = "Programa hecho por Alejandro Rafael Alpizar Rodriguez para Nueva Cinema SA, con sede en Perez Zeledon\nTodos los derechos reservados ®";
}
void Archive::getInfo() {
	cout << string(info);
}

