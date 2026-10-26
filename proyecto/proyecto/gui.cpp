#include "gui.h"
#include <algorithm>
#include "dateTime.h"
#include "palette.h"

namespace {
	const unsigned int windowWidth = 1000;
	const unsigned int windowHeight = 700;
	const float centerX = 500;
	const float rowHeight = 60;
	const int moviesPerPage = 4;
	const int minimumYear = 1888;
	const int maximumYear = 2100;
	const int maximumDuration = 600;
	const int maximumRoomNumber = 99;
	const int maximumSeats = ROWSTHEATER * COLUMNSTHEATER;
	const int maximumPrice = 100000;
	const int secondsPerMinute = 60;
	const sf::Uint32 backspaceKey = 8;
	const sf::Uint32 tabKey = 9;
	const sf::Uint32 firstPrintableKey = 32;
	const sf::Uint32 deleteKey = 127;
	const sf::FloatRect listArea(40, 110, 440, 480);
	const sf::FloatRect fullListArea(40, 110, 920, 480);
	const sf::FloatRect formArea(510, 110, 450, 480);
	const sf::FloatRect backButtonArea(40, 620, 160, 48);
	const float formLeft = 534;
	const float formWidth = 402;
	const float formTop = 158;
	const float formRowHeight = 72;
	const float fieldHeight = 40;
}

Gui::Gui(const std::vector<Movies>& initialMovies, const std::vector<Room>& initialRooms, const std::vector<movieTheater>& initialTheaters)
	: window(sf::VideoMode(windowWidth, windowHeight), "Nueva Cinema", sf::Style::Titlebar | sf::Style::Close),
	widgets(window), movies(initialMovies), rooms(initialRooms), theaters(initialTheaters), screen(Screen::MainMenu),
	isMessageError(false), listOffset(0), moviePage(0), movieIndex(0), roomNumber(0), theaterIndex(0),
	selectedSeats(maximumSeats, false), lastVoucher(0), foundVoucher(0), formMovieIndex(0), formRoomIndex(0), now(0) {
	window.setFramerateLimit(60);
	registerScreens();
	registerForms();
}
bool Gui::loadFont(const std::string& path) {
	return widgets.loadFont(path);
}
void Gui::run() {
	while (window.isOpen()) {
		bool isClicked = false;
		sf::Event event;
		while (window.pollEvent(event)) {
			isClicked = handleEvent(event) || isClicked;
		}
		now = std::time(nullptr);
		widgets.beginFrame(isClicked);
		window.clear(Palette::background);
		screens[screen]();
		widgets.endFrame();
		window.display();
	}
}
void Gui::registerScreens() {
	screens[Screen::MainMenu] = [this]() { drawMainMenu(); };
	screens[Screen::FileMenu] = [this]() { drawFileMenu(); };
	screens[Screen::About] = [this]() { drawAbout(); };
	screens[Screen::MaintenanceMenu] = [this]() { drawMaintenanceMenu(); };
	screens[Screen::MovieMaintenance] = [this]() { drawMovieMaintenance(); };
	screens[Screen::RoomMaintenance] = [this]() { drawRoomMaintenance(); };
	screens[Screen::ScheduleMaintenance] = [this]() { drawScheduleMaintenance(); };
	screens[Screen::ReservationMovies] = [this]() { drawReservationMovies(); };
	screens[Screen::ReservationRooms] = [this]() { drawReservationRooms(); };
	screens[Screen::ReservationSchedules] = [this]() { drawReservationSchedules(); };
	screens[Screen::ReservationSeats] = [this]() { drawReservationSeats(); };
	screens[Screen::ReservationTicket] = [this]() { drawReservationTicket(); };
	screens[Screen::Sale] = [this]() { drawSale(); };
}
void Gui::registerForms() {
	formLayouts[Screen::MovieMaintenance] = { { "", false, 60 }, { "", true, 4 }, { "", true, 3 }, { "", false, 40 }, { "", false, 120 } };
	formLayouts[Screen::RoomMaintenance] = { { "", true, 2 }, { "", true, 2 }, { "", true, 6 } };
	formLayouts[Screen::ScheduleMaintenance] = { { "", false, 10 }, { "", false, 5 }, { "", false, 5 } };
	formLayouts[Screen::Sale] = { { "", true, 8 }, { "", true, 12 }, { "", true, 16 } };
}
bool Gui::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		openScreen(Screen::MainMenu);
	}
	if (event.type == sf::Event::TextEntered) {
		handleTextInput(event.text.unicode);
	}
	if (event.type == sf::Event::MouseWheelScrolled) {
		listOffset -= (int)event.mouseWheelScroll.delta;
	}
	return event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left;
}
void Gui::handleTextInput(sf::Uint32 character) {
	int focused = widgets.getFocusedField();
	if (focused < 0 || focused >= (int)fields.size()) {
		return;
	}
	if (character == tabKey) {
		widgets.setFocusedField((focused + 1) % (int)fields.size());
		return;
	}
	if (character == backspaceKey) {
		removeLastCharacter(fields[focused].value);
		return;
	}
	if (canAppend(fields[focused], character)) {
		fields[focused].value += Widgets::toUtf8(sf::String(character));
	}
}
bool Gui::canAppend(const FormField& field, sf::Uint32 character) const {
	bool isPrintable = character >= firstPrintableKey && character != deleteKey;
	bool isDigit = character >= '0' && character <= '9';
	bool hasRoom = Widgets::fromUtf8(field.value).getSize() < field.maxLength;
	return isPrintable && hasRoom && (isDigit || !field.isDigitsOnly);
}
void Gui::openScreen(Screen next) {
	screen = next;
	listOffset = 0;
	message.clear();
	std::map<Screen, std::vector<FormField>>::const_iterator layout = formLayouts.find(next);
	fields = layout == formLayouts.end() ? std::vector<FormField>() : layout->second;
	widgets.setFocusedField(fields.empty() ? -1 : 0);
}
void Gui::openScheduleMaintenance() {
	openScreen(Screen::ScheduleMaintenance);
	fields[0].value = DateTime::formatDate(std::time(nullptr));
	formMovieIndex = 0;
	formRoomIndex = 0;
}
void Gui::openSale(int voucher) {
	openScreen(Screen::Sale);
	foundVoucher = 0;
	if (voucher > 0) {
		fields[0].value = std::to_string(voucher);
		searchReservation();
	}
}
void Gui::showMessage(const std::string& text, bool isError) {
	message = text;
	isMessageError = isError;
}
void Gui::drawMessage(float x, float y, float width) {
	if (!message.empty()) {
		widgets.drawText(message, x, y, 16, isMessageError ? Palette::accentHover : Palette::success, false, width);
	}
}
void Gui::drawHeader(const std::string& title, const std::string& subtitle) {
	widgets.drawText(title, 40, 30, 32, Palette::text, true);
	widgets.drawText(subtitle, 40, 76, 17, Palette::muted);
}
void Gui::drawBackButton(Screen target) {
	if (widgets.drawButton("Volver", backButtonArea)) {
		openScreen(target);
	}
}
void Gui::drawMenu(const std::string& title, const std::string& subtitle, const std::vector<MenuOption>& options) {
	widgets.drawCenteredText(title, centerX, 110, 52, Palette::accent, true);
	widgets.drawCenteredText(subtitle, centerX, 168, 22, Palette::muted);
	for (std::size_t index = 0; index < options.size(); index++) {
		sf::FloatRect area(centerX - 160, 230 + index * 72.f, 320, 56);
		if (widgets.drawButton(options[index].label, area)) {
			options[index].action();
			return;
		}
	}
}
void Gui::drawScrollableList(const sf::FloatRect& area, int count, const std::string& emptyText, const std::function<void(int, const sf::FloatRect&)>& drawRow) {
	widgets.drawPanel(area, Palette::panel);
	int visibleRows = (int)(area.height / rowHeight);
	listOffset = std::max(0, std::min(listOffset, count - visibleRows));
	if (count == 0) {
		widgets.drawCenteredText(emptyText, area.left + area.width / 2, area.top + area.height / 2, 18, Palette::muted);
		return;
	}
	for (int row = 0; row < visibleRows && listOffset + row < count; row++) {
		drawRow(listOffset + row, sf::FloatRect(area.left, area.top + row * rowHeight, area.width, rowHeight));
	}
	if (count > visibleRows) {
		widgets.drawText("Use la rueda del mouse para ver más", area.left, area.top + area.height + 6, 14, Palette::muted);
	}
}
void Gui::drawFormField(int field, const std::string& label, int row) {
	widgets.drawTextField(field, label, fields[field].value, sf::FloatRect(formLeft, formTop + row * formRowHeight, formWidth, fieldHeight));
}
int Gui::drawFormSelector(const std::string& label, const std::string& value, int row) {
	return widgets.drawSelector(label, value, sf::FloatRect(formLeft, formTop + row * formRowHeight, formWidth, fieldHeight));
}
bool Gui::drawFormButton(const std::string& label) {
	return widgets.drawButton(label, sf::FloatRect(formLeft, formArea.top + formArea.height - 68, formWidth, 48));
}
void Gui::drawMainMenu() {
	drawMenu("NUEVA CINEMA", "Bienvenido a la app de Nueva Cinema Pérez Zeledón", {
		{ "Archivo", [this]() { openScreen(Screen::FileMenu); } },
		{ "Mantenimiento", [this]() { openScreen(Screen::MaintenanceMenu); } },
		{ "Reserva", [this]() {
			moviePage = 0;
			openScreen(Screen::ReservationMovies);
		} },
		{ "Venta", [this]() { openSale(0); } }
	});
}
void Gui::drawFileMenu() {
	drawMenu("Archivo", "Información del sistema", {
		{ "Acerca de", [this]() { openScreen(Screen::About); } },
		{ "Salir", [this]() { window.close(); } },
		{ "Volver", [this]() { openScreen(Screen::MainMenu); } }
	});
}
void Gui::drawAbout() {
	drawHeader("Acerca de", "Nueva Cinema SA");
	widgets.drawPanel(sf::FloatRect(40, 110, 920, 200), Palette::panel);
	widgets.drawText(archive.getInfo(), 70, 140, 22, Palette::text, false, 860);
	drawBackButton(Screen::FileMenu);
}
void Gui::drawMaintenanceMenu() {
	drawMenu("Mantenimiento", "Registro de películas, salas y horarios", {
		{ "Películas", [this]() { openScreen(Screen::MovieMaintenance); } },
		{ "Salas", [this]() { openScreen(Screen::RoomMaintenance); } },
		{ "Horarios", [this]() { openScheduleMaintenance(); } },
		{ "Volver", [this]() { openScreen(Screen::MainMenu); } }
	});
}
void Gui::drawMovieMaintenance() {
	drawHeader("Mantenimiento · Películas", "Registre las películas que se exhibirán");
	drawScrollableList(listArea, (int)movies.size(), "No hay películas registradas", [this](int index, const sf::FloatRect& row) {
		widgets.drawListRow(row, movies[index].getName(), describeMovie(movies[index]), "", Palette::muted);
	});
	widgets.drawPanel(formArea, Palette::panel);
	drawFormField(0, "Nombre", 0);
	drawFormField(1, "Año", 1);
	drawFormField(2, "Duración (min)", 2);
	drawFormField(3, "País", 3);
	drawFormField(4, "Reseña", 4);
	drawMessage(formLeft, formTop + 4 * formRowHeight + fieldHeight + 6, formWidth);
	if (drawFormButton("Guardar película")) {
		saveMovie();
	}
	drawBackButton(Screen::MaintenanceMenu);
}
void Gui::drawRoomMaintenance() {
	drawHeader("Mantenimiento · Salas", "Registre las salas, su cantidad de butacas y el precio");
	drawScrollableList(listArea, (int)rooms.size(), "No hay salas registradas", [this](int index, const sf::FloatRect& row) {
		widgets.drawListRow(row, "Sala " + std::to_string(rooms[index].getNumber()), describeRoom(rooms[index].getSeatCount(), rooms[index].getPrice()), "", Palette::muted);
	});
	widgets.drawPanel(formArea, Palette::panel);
	drawFormField(0, "Número de sala", 0);
	drawFormField(1, "Cantidad de butacas (máximo " + std::to_string(maximumSeats) + ")", 1);
	drawFormField(2, "Precio por butaca", 2);
	drawMessage(formLeft, formTop + 2 * formRowHeight + fieldHeight + 6, formWidth);
	if (drawFormButton("Guardar sala")) {
		saveRoom();
	}
	drawBackButton(Screen::MaintenanceMenu);
}
void Gui::drawScheduleMaintenance() {
	drawHeader("Mantenimiento · Horarios", "Asigne a cada película una sala, una fecha y sus horas");
	drawScrollableList(listArea, (int)theaters.size(), "No hay horarios registrados", [this](int index, const sf::FloatRect& row) {
		const movieTheater& theater = theaters[index];
		std::string seats = std::to_string(theater.freeSeats()) + "/" + std::to_string(theater.getSeatCount());
		widgets.drawListRow(row, theater.getName(), describeSchedule(theater), seats, Palette::muted);
	});
	widgets.drawPanel(formArea, Palette::panel);
	drawScheduleForm();
	drawBackButton(Screen::MaintenanceMenu);
}
void Gui::drawScheduleForm() {
	if (movies.empty() || rooms.empty()) {
		widgets.drawCenteredText("Registre primero una película y una sala", formArea.left + formArea.width / 2, formArea.top + formArea.height / 2, 18, Palette::muted);
		return;
	}
	int movieCount = (int)movies.size();
	int roomCount = (int)rooms.size();
	formMovieIndex %= movieCount;
	formRoomIndex %= roomCount;
	int movieStep = drawFormSelector("Película", movies[formMovieIndex].getName(), 0);
	int roomStep = drawFormSelector("Sala", "Sala " + std::to_string(rooms[formRoomIndex].getNumber()), 1);
	formMovieIndex = (formMovieIndex + movieStep + movieCount) % movieCount;
	formRoomIndex = (formRoomIndex + roomStep + roomCount) % roomCount;
	drawFormField(0, "Fecha (dd/mm/aaaa)", 2);
	drawFormField(1, "Hora inicial (hh:mm)", 3);
	drawFormField(2, "Hora final (hh:mm)", 4);
	drawMessage(formLeft, formTop + 4 * formRowHeight + fieldHeight + 6, formWidth);
	if (drawFormButton("Guardar horario")) {
		saveSchedule();
	}
}
void Gui::drawReservationMovies() {
	drawHeader("Reserva · Películas", "Seleccione la película que desea ver");
	int count = (int)movies.size();
	int pages = std::max(1, (count + moviesPerPage - 1) / moviesPerPage);
	moviePage = std::min(moviePage, pages - 1);
	if (count == 0) {
		widgets.drawCenteredText("No hay películas registradas", centerX, 340, 20, Palette::muted);
	}
	for (int slot = 0; slot < moviesPerPage && moviePage * moviesPerPage + slot < count; slot++) {
		sf::FloatRect area(40 + (slot % 2) * 475.f, 110 + (slot / 2) * 245.f, 445, 225);
		drawMovieCard(moviePage * moviesPerPage + slot, area);
	}
	if (pages > 1) {
		widgets.drawCenteredText("Página " + std::to_string(moviePage + 1) + " de " + std::to_string(pages), 620, 644, 16, Palette::muted);
		moviePage -= widgets.drawButton("Anterior", sf::FloatRect(710, 620, 120, 48), moviePage > 0) ? 1 : 0;
		moviePage += widgets.drawButton("Siguiente", sf::FloatRect(840, 620, 120, 48), moviePage < pages - 1) ? 1 : 0;
	}
	drawBackButton(Screen::MainMenu);
}
void Gui::drawMovieCard(int index, const sf::FloatRect& area) {
	const Movies& movie = movies[index];
	int upcoming = 0;
	for (int room : findRoomNumbers(index)) {
		upcoming += countUpcoming(findTheaters(index, room));
	}
	bool isAvailable = upcoming > 0;
	float textWidth = area.width - 40;
	widgets.drawPanel(area, isAvailable && widgets.isHovered(area) ? Palette::panelHover : Palette::panel);
	float line = widgets.drawText(movie.getName(), area.left + 20, area.top + 16, 22, Palette::text, true, textWidth);
	line = widgets.drawText(describeMovie(movie), area.left + 20, line + 8, 17, Palette::muted);
	widgets.drawText(widgets.fitText("Reseña: " + movie.getReview(), 17, textWidth), area.left + 20, line + 6, 17, Palette::muted);
	std::string status = isAvailable ? formatCount(upcoming, "función disponible", "funciones disponibles") : "Sin funciones disponibles";
	widgets.drawText(status, area.left + 20, area.top + area.height - 38, 17, isAvailable ? Palette::gold : Palette::accentHover, true);
	if (isAvailable && widgets.isPressed(area)) {
		movieIndex = index;
		openScreen(Screen::ReservationRooms);
	}
}
void Gui::drawReservationRooms() {
	drawHeader("Reserva · Sala", movies[movieIndex].getName() + " · seleccione la sala");
	std::vector<int> roomNumbers = findRoomNumbers(movieIndex);
	drawScrollableList(fullListArea, (int)roomNumbers.size(), "Esta película no tiene salas asignadas", [this, &roomNumbers](int index, const sf::FloatRect& row) {
		std::vector<int> roomTheaters = findTheaters(movieIndex, roomNumbers[index]);
		const movieTheater& first = theaters[roomTheaters[0]];
		int upcoming = countUpcoming(roomTheaters);
		std::string status = upcoming > 0 ? formatCount(upcoming, "función", "funciones") : "Sin funciones";
		if (widgets.drawListRow(row, "Sala " + std::to_string(roomNumbers[index]), describeRoom(first.getSeatCount(), first.getPrice()), status, upcoming > 0 ? Palette::success : Palette::muted, upcoming > 0)) {
			roomNumber = roomNumbers[index];
			openScreen(Screen::ReservationSchedules);
		}
	});
	drawBackButton(Screen::ReservationMovies);
}
void Gui::drawReservationSchedules() {
	drawHeader("Reserva · Horario", movies[movieIndex].getName() + " · Sala " + std::to_string(roomNumber) + " · seleccione el horario");
	std::vector<int> roomTheaters = findTheaters(movieIndex, roomNumber);
	drawScrollableList(fullListArea, (int)roomTheaters.size(), "No hay horarios para esta sala", [this, &roomTheaters](int index, const sf::FloatRect& row) {
		drawScheduleRow(roomTheaters[index], row);
	});
	drawBackButton(Screen::ReservationRooms);
}
void Gui::drawScheduleRow(int index, const sf::FloatRect& area) {
	const movieTheater& theater = theaters[index];
	std::string detail = "Butacas disponibles: " + std::to_string(theater.freeSeats()) + "/" + std::to_string(theater.getSeatCount());
	std::string status = "Disponible";
	sf::Color statusColor = Palette::success;
	if (theater.fullRoom()) {
		status = "Sala llena";
		statusColor = Palette::accentHover;
	}
	if (theater.hasStarted(now)) {
		status = "Iniciada";
		statusColor = Palette::muted;
	}
	bool isOpen = !theater.fullRoom() && !theater.hasStarted(now);
	if (widgets.drawListRow(area, formatScheduleTime(theater), detail, status, statusColor, isOpen)) {
		theaterIndex = index;
		selectedSeats.assign(maximumSeats, false);
		openScreen(Screen::ReservationSeats);
	}
}
void Gui::drawReservationSeats() {
	const movieTheater& theater = theaters[theaterIndex];
	drawHeader("Reserva · Butacas", "Seleccione las butacas disponibles");
	drawSeatGrid(theater);
	drawReservationSummary(theater);
	drawBackButton(Screen::ReservationSchedules);
}
void Gui::drawSeatGrid(const movieTheater& theater) {
	int rowCount = (theater.getSeatCount() + COLUMNSTHEATER - 1) / COLUMNSTHEATER;
	const float gap = 10;
	const float top = 160;
	const float cell = 62;
	float gridWidth = COLUMNSTHEATER * cell + (COLUMNSTHEATER - 1) * gap;
	float left = 40 + (440 - gridWidth) / 2;
	widgets.drawCenteredText("PANTALLA", left + gridWidth / 2, top - 46, 13, Palette::muted);
	widgets.drawPanel(sf::FloatRect(left, top - 32, gridWidth, 6), Palette::muted);
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < COLUMNSTHEATER; j++) {
			sf::FloatRect area(left + j * (cell + gap), top + i * (cell + gap), cell, cell);
			drawSeat(theater.getSeat(i, j), i * COLUMNSTHEATER + j + 1, area);
		}
	}
	drawSeatLegend(top + 392);
}
void Gui::drawSeat(const Seats& seat, int number, const sf::FloatRect& area) {
	if (!seat.isEnabled()) {
		return;
	}
	bool isSelected = selectedSeats[number - 1];
	std::string label = std::to_string(number);
	if (seat.isReserved()) {
		label = "R";
	}
	if (seat.isSold()) {
		label = "V";
	}
	widgets.drawPanel(area, chooseSeatColor(seat, isSelected, widgets.isHovered(area)));
	sf::Color labelColor = seat.isReserved() ? Palette::background : Palette::text;
	widgets.drawCenteredText(label, area.left + area.width / 2, area.top + area.height / 2, 20, labelColor, true);
	if (seat.isAvailable() && widgets.isPressed(area)) {
		selectedSeats[number - 1] = !isSelected;
	}
}
void Gui::drawSeatLegend(float y) {
	const std::vector<std::pair<std::string, sf::Color>> items = {
		{ "Disponible", Palette::available },
		{ "Seleccionada", Palette::selected },
		{ "Reservada (R)", Palette::reserved },
		{ "Vendida (V)", Palette::sold }
	};
	float x = 40;
	for (const std::pair<std::string, sf::Color>& item : items) {
		widgets.drawPanel(sf::FloatRect(x, y, 16, 16), item.second);
		widgets.drawText(item.first, x + 22, y - 3, 15, Palette::muted);
		x += 22 + widgets.measureText(item.first, 15) + 16;
	}
}
void Gui::drawReservationSummary(const movieTheater& theater) {
	sf::FloatRect side(520, 110, 440, 480);
	float x = side.left + 24;
	float textWidth = side.width - 48;
	int count = countSelectedSeats();
	widgets.drawPanel(side, Palette::panel);
	float line = widgets.drawText("Sala " + std::to_string(theater.getNumber()), x, side.top + 22, 16, Palette::accent, true);
	line = widgets.drawText(theater.getName(), x, line + 6, 24, Palette::text, true, textWidth);
	line = widgets.drawText(formatScheduleTime(theater), x, line + 8, 17, Palette::muted);
	line = widgets.drawText("Boletos: " + std::to_string(count), x, line + 30, 20, Palette::text);
	line = widgets.drawText(widgets.fitText("Butacas: " + (count == 0 ? "ninguna" : joinSeats(collectSelectedSeats())), 20, textWidth), x, line + 10, 20, Palette::text);
	line = widgets.drawText("Precio por butaca: " + std::to_string(theater.getPrice()), x, line + 10, 20, Palette::text);
	widgets.drawText("Total: " + std::to_string(count * theater.getPrice()), x, line + 20, 28, Palette::gold, true);
	drawMessage(x, side.top + side.height - 112, textWidth);
	if (widgets.drawButton("Confirmar reserva", sf::FloatRect(x, side.top + side.height - 76, textWidth, 52), count > 0)) {
		confirmReservation();
	}
}
void Gui::drawReservationTicket() {
	const movieTheater& theater = theaters[book.getTheater(lastVoucher)];
	sf::FloatRect card(centerX - 270, 80, 540, 460);
	float x = card.left + 40;
	float textWidth = card.width - 80;
	widgets.drawPanel(card, Palette::panel);
	widgets.drawCenteredText("Reserva hecha", centerX, card.top + 46, 32, Palette::success, true);
	float line = widgets.drawText("Número de consecutivo: " + std::to_string(lastVoucher), x, card.top + 96, 24, Palette::gold, true);
	line = widgets.drawText("Película: " + theater.getName(), x, line + 16, 20, Palette::text, false, textWidth);
	line = widgets.drawText(describeSchedule(theater), x, line + 10, 20, Palette::text, false, textWidth);
	line = widgets.drawText("Butacas: " + lastSeats, x, line + 10, 20, Palette::text, false, textWidth);
	widgets.drawText("Monto a pagar: " + std::to_string(book.getTotal(lastVoucher)), x, line + 18, 26, Palette::gold, true);
	widgets.drawText("Presente este consecutivo en Venta hasta 30 minutos antes de la función", x, card.top + card.height - 64, 16, Palette::muted, false, textWidth);
	if (widgets.drawButton("Ir a Venta", sf::FloatRect(card.left, card.top + card.height + 24, 255, 52))) {
		openSale(lastVoucher);
	}
	if (widgets.drawButton("Volver al menú", sf::FloatRect(card.left + card.width - 255, card.top + card.height + 24, 255, 52))) {
		openScreen(Screen::MainMenu);
	}
}
void Gui::drawSale() {
	drawHeader("Venta", "Digite el número de consecutivo de la reserva para realizar el pago");
	sf::FloatRect reservationArea(40, 110, 440, 480);
	sf::FloatRect customerArea(510, 110, 450, 480);
	widgets.drawPanel(reservationArea, Palette::panel);
	widgets.drawPanel(customerArea, Palette::panel);
	widgets.drawTextField(0, "Número de consecutivo", fields[0].value, sf::FloatRect(64, 158, 250, fieldHeight));
	if (widgets.drawButton("Buscar", sf::FloatRect(330, 158, 126, fieldHeight), !fields[0].value.empty())) {
		searchReservation();
	}
	drawSaleDetails(sf::FloatRect(64, 228, 392, 340));
	drawCustomerForm(customerArea);
	drawBackButton(Screen::MainMenu);
}
void Gui::drawSaleDetails(const sf::FloatRect& area) {
	if (!book.detectVoucher(foundVoucher)) {
		widgets.drawText("Busque una reserva para ver su detalle", area.left, area.top, 17, Palette::muted, false, area.width);
		return;
	}
	const movieTheater& theater = theaters[book.getTheater(foundVoucher)];
	std::pair<std::string, sf::Color> status = describeSaleStatus(foundVoucher);
	float line = widgets.drawText(theater.getName(), area.left, area.top, 22, Palette::text, true, area.width);
	line = widgets.drawText(describeSchedule(theater), area.left, line + 8, 17, Palette::muted, false, area.width);
	line = widgets.drawText("Boletos: " + std::to_string(book.getTickets(foundVoucher)), area.left, line + 6, 17, Palette::muted, false, area.width);
	line = widgets.drawText("Monto a pagar: " + std::to_string(book.getTotal(foundVoucher)), area.left, line + 14, 26, Palette::gold, true);
	widgets.drawText(status.first, area.left, line + 18, 18, status.second, true, area.width);
}
void Gui::drawCustomerForm(const sf::FloatRect& area) {
	float x = area.left + 24;
	float fieldWidth = area.width - 48;
	widgets.drawText("Datos del cliente", x, area.top + 20, 20, Palette::text, true);
	if (sale.detectSale(foundVoucher)) {
		drawPaidSale(sale.getCustomer(foundVoucher), x, area.top + 70);
		drawMessage(x, area.top + 240, fieldWidth);
		return;
	}
	widgets.drawTextField(1, "Cédula", fields[1].value, sf::FloatRect(x, area.top + 92, fieldWidth, fieldHeight));
	widgets.drawTextField(2, "Número de tarjeta", fields[2].value, sf::FloatRect(x, area.top + 164, fieldWidth, fieldHeight));
	widgets.drawText("La cédula lleva de 9 a 12 dígitos y la tarjeta 16 dígitos", x, area.top + 214, 14, Palette::muted, false, fieldWidth);
	drawMessage(x, area.top + 250, fieldWidth);
	if (widgets.drawButton("Pagar", sf::FloatRect(x, area.top + area.height - 76, fieldWidth, 52), canSellVoucher(foundVoucher))) {
		payReservation();
	}
}
void Gui::drawPaidSale(const Customer& customer, float x, float y) {
	float line = widgets.drawText("Cédula: " + customer.getID(), x, y, 18, Palette::text);
	line = widgets.drawText("Tarjeta: " + customer.getMaskedCard(), x, line + 8, 18, Palette::text);
	widgets.drawText("Tiquetes vendidos", x, line + 20, 22, Palette::success, true);
}
void Gui::saveMovie() {
	int year = toNumber(fields[1].value);
	int duration = toNumber(fields[2].value);
	if (fields[0].value.empty() || fields[3].value.empty() || fields[4].value.empty()) {
		showMessage("Complete todos los datos de la película", true);
		return;
	}
	if (year < minimumYear || year > maximumYear) {
		showMessage("Digite un año entre " + std::to_string(minimumYear) + " y " + std::to_string(maximumYear), true);
		return;
	}
	if (duration < 1 || duration > maximumDuration) {
		showMessage("Digite una duración entre 1 y " + std::to_string(maximumDuration) + " minutos", true);
		return;
	}
	movies.push_back(Movies(fields[0].value, year, duration, fields[3].value, fields[4].value));
	openScreen(Screen::MovieMaintenance);
	listOffset = (int)movies.size();
	showMessage("Película guardada con éxito", false);
}
void Gui::saveRoom() {
	int number = toNumber(fields[0].value);
	int seatCount = toNumber(fields[1].value);
	int price = toNumber(fields[2].value);
	if (number < 1 || number > maximumRoomNumber) {
		showMessage("Digite un número de sala entre 1 y " + std::to_string(maximumRoomNumber), true);
		return;
	}
	if (seatCount < 1 || seatCount > maximumSeats) {
		showMessage("Digite una cantidad de butacas entre 1 y " + std::to_string(maximumSeats), true);
		return;
	}
	if (price < 1 || price > maximumPrice) {
		showMessage("Digite un precio entre 1 y " + std::to_string(maximumPrice), true);
		return;
	}
	if (hasRoomNumber(number)) {
		showMessage("Ya existe una sala con el número " + std::to_string(number), true);
		return;
	}
	rooms.push_back(Room(number, seatCount, price));
	openScreen(Screen::RoomMaintenance);
	listOffset = (int)rooms.size();
	showMessage("Sala guardada con éxito", false);
}
void Gui::saveSchedule() {
	std::time_t start = 0;
	std::time_t end = 0;
	bool hasValidTimes = DateTime::parse(fields[0].value, fields[1].value, start) && DateTime::parse(fields[0].value, fields[2].value, end);
	if (!hasValidTimes) {
		showMessage("Revise la fecha (dd/mm/aaaa) y las horas (hh:mm)", true);
		return;
	}
	const Movies& movie = movies[formMovieIndex];
	const Room& room = rooms[formRoomIndex];
	if (end - start < movie.getDuration() * secondsPerMinute) {
		showMessage("La función debe durar al menos " + std::to_string(movie.getDuration()) + " minutos", true);
		return;
	}
	if (start <= now) {
		showMessage("La función debe iniciar en una fecha y hora futuras", true);
		return;
	}
	movieTheater candidate(movie.getName(), movie.getYear(), movie.getDuration(), movie.getCountry(), movie.getReview(), room.getNumber(), room.getPrice(), room.getSeatCount(), start, end);
	for (const movieTheater& theater : theaters) {
		if (theater.overlaps(candidate)) {
			showMessage("La sala ya tiene una función en ese horario", true);
			return;
		}
	}
	theaters.push_back(candidate);
	std::string date = fields[0].value;
	openScreen(Screen::ScheduleMaintenance);
	fields[0].value = date;
	listOffset = (int)theaters.size();
	showMessage("Horario guardado con éxito", false);
}
void Gui::confirmReservation() {
	movieTheater& theater = theaters[theaterIndex];
	std::vector<int> seats = collectSelectedSeats();
	if (theater.hasStarted(now)) {
		showMessage("La función ya inició, elija otro horario", true);
		return;
	}
	int voucher = book.toBooking(theaterIndex, (int)seats.size(), (int)seats.size() * theater.getPrice());
	if (voucher == 0) {
		showMessage("No hay espacio para más reservas", true);
		return;
	}
	for (int seat : seats) {
		theater.setReserved(seat, voucher);
	}
	lastVoucher = voucher;
	lastSeats = joinSeats(seats);
	openScreen(Screen::ReservationTicket);
}
void Gui::searchReservation() {
	int voucher = toNumber(fields[0].value);
	foundVoucher = 0;
	fields[1].value.clear();
	fields[2].value.clear();
	if (!book.detectVoucher(voucher)) {
		showMessage("Voucher inexistente: no hay una reserva con ese consecutivo", true);
		return;
	}
	foundVoucher = voucher;
	message.clear();
	widgets.setFocusedField(1);
}
void Gui::payReservation() {
	if (!sale.isValidCustomer(fields[1].value, fields[2].value)) {
		showMessage("Revise los datos: la cédula lleva de 9 a 12 dígitos y la tarjeta 16", true);
		return;
	}
	if (!canSellVoucher(foundVoucher) || !sale.pay(foundVoucher, fields[1].value, fields[2].value)) {
		showMessage("No fue posible realizar el pago de esta reserva", true);
		return;
	}
	theaters[book.getTheater(foundVoucher)].sold(foundVoucher);
	book.setPaid(foundVoucher);
	showMessage("Pago realizado con éxito", false);
}
bool Gui::hasRoomNumber(int number) const {
	for (const Room& room : rooms) {
		if (room.getNumber() == number) {
			return true;
		}
	}
	return false;
}
bool Gui::canSellVoucher(int voucher) const {
	return book.detectVoucher(voucher) && !book.isPaid(voucher) && theaters[book.getTheater(voucher)].canSell(now);
}
std::vector<int> Gui::findRoomNumbers(int movie) const {
	std::vector<int> numbers;
	for (const movieTheater& theater : theaters) {
		bool isListed = std::find(numbers.begin(), numbers.end(), theater.getNumber()) != numbers.end();
		if (theater.getName() == movies[movie].getName() && !isListed) {
			numbers.push_back(theater.getNumber());
		}
	}
	return numbers;
}
std::vector<int> Gui::findTheaters(int movie, int room) const {
	std::vector<int> indexes;
	for (int index = 0; index < (int)theaters.size(); index++) {
		if (theaters[index].getName() == movies[movie].getName() && theaters[index].getNumber() == room) {
			indexes.push_back(index);
		}
	}
	return indexes;
}
int Gui::countUpcoming(const std::vector<int>& theaterIndexes) const {
	int total = 0;
	for (int index : theaterIndexes) {
		total += theaters[index].hasStarted(now) ? 0 : 1;
	}
	return total;
}
int Gui::countSelectedSeats() const {
	return (int)std::count(selectedSeats.begin(), selectedSeats.end(), true);
}
std::vector<int> Gui::collectSelectedSeats() const {
	std::vector<int> seats;
	for (std::size_t index = 0; index < selectedSeats.size(); index++) {
		if (selectedSeats[index]) {
			seats.push_back((int)index + 1);
		}
	}
	return seats;
}
sf::Color Gui::chooseSeatColor(const Seats& seat, bool isSelected, bool isHovered) const {
	if (seat.isSold()) {
		return Palette::sold;
	}
	if (seat.isReserved()) {
		return Palette::reserved;
	}
	if (isSelected) {
		return Palette::selected;
	}
	return isHovered ? Palette::availableHover : Palette::available;
}
std::pair<std::string, sf::Color> Gui::describeSaleStatus(int voucher) const {
	if (book.isPaid(voucher)) {
		return { "Pagada", Palette::success };
	}
	if (!theaters[book.getTheater(voucher)].canSell(now)) {
		return { "Venta cerrada: faltan menos de 30 minutos para la función", Palette::accentHover };
	}
	return { "Pendiente de pago", Palette::gold };
}
std::string Gui::describeMovie(const Movies& movie) const {
	return std::to_string(movie.getYear()) + "  ·  " + std::to_string(movie.getDuration()) + " min  ·  " + movie.getCountry();
}
std::string Gui::describeRoom(int seatCount, int price) const {
	return formatCount(seatCount, "butaca", "butacas") + "  ·  Precio por butaca: " + std::to_string(price);
}
std::string Gui::describeSchedule(const movieTheater& theater) const {
	return "Sala " + std::to_string(theater.getNumber()) + "  ·  " + formatScheduleTime(theater);
}
std::string Gui::formatScheduleTime(const movieTheater& theater) const {
	return DateTime::formatDate(theater.getStart()) + "  ·  " + DateTime::formatTime(theater.getStart()) + " - " + DateTime::formatTime(theater.getEnd());
}
std::string Gui::joinSeats(const std::vector<int>& seats) {
	std::string result;
	for (std::size_t index = 0; index < seats.size(); index++) {
		result += (index > 0 ? ", " : "") + std::to_string(seats[index]);
	}
	return result;
}
std::string Gui::formatCount(int count, const std::string& singular, const std::string& plural) {
	return std::to_string(count) + " " + (count == 1 ? singular : plural);
}
int Gui::toNumber(const std::string& value) {
	if (value.empty() || value.size() > 9) {
		return -1;
	}
	return std::stoi(value);
}
void Gui::removeLastCharacter(std::string& value) {
	sf::String text = Widgets::fromUtf8(value);
	if (!text.isEmpty()) {
		text.erase(text.getSize() - 1);
	}
	value = Widgets::toUtf8(text);
}
