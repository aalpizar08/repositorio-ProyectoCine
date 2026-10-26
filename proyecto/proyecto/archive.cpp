#include "archive.h"
Archive::Archive() {
	this->info = "Programa hecho por Alejandro Rafael Alpizar Rodriguez para Nueva Cinema SA, con sede en Pérez Zeledón\nTodos los derechos reservados ®";
}
string Archive::getInfo() const {
	return info;
}
