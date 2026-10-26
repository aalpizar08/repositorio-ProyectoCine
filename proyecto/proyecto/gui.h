#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "archive.h"
#include "booking.h"
#include "formField.h"
#include "menuOption.h"
#include "movieTheater.h"
#include "movies.h"
#include "room.h"
#include "sales.h"
#include "screen.h"
#include "widgets.h"

class Gui {
private:
	sf::RenderWindow window;
	Widgets widgets;
	std::vector<Movies> movies;
	std::vector<Room> rooms;
	std::vector<movieTheater> theaters;
	Booking book;
	Sales sale;
	Archive archive;
	Screen screen;
	std::map<Screen, std::function<void()>> screens;
	std::map<Screen, std::vector<FormField>> formLayouts;
	std::vector<FormField> fields;
	std::string message;
	bool isMessageError;
	int listOffset;
	int moviePage;
	int movieIndex;
	int roomNumber;
	int theaterIndex;
	std::vector<bool> selectedSeats;
	int lastVoucher;
	std::string lastSeats;
	int foundVoucher;
	int formMovieIndex;
	int formRoomIndex;
	std::time_t now;
public:
	Gui(const std::vector<Movies>& initialMovies, const std::vector<Room>& initialRooms, const std::vector<movieTheater>& initialTheaters);
	bool loadFont(const std::string& path);
	void run();
private:
	void registerScreens();
	void registerForms();
	bool handleEvent(const sf::Event& event);
	void handleTextInput(sf::Uint32 character);
	bool canAppend(const FormField& field, sf::Uint32 character) const;
	void openScreen(Screen next);
	void openScheduleMaintenance();
	void openSale(int voucher);
	void showMessage(const std::string& text, bool isError);
	void drawMessage(float x, float y, float width);
	void drawHeader(const std::string& title, const std::string& subtitle);
	void drawBackButton(Screen target);
	void drawMenu(const std::string& title, const std::string& subtitle, const std::vector<MenuOption>& options);
	void drawScrollableList(const sf::FloatRect& area, int count, const std::string& emptyText, const std::function<void(int, const sf::FloatRect&)>& drawRow);
	void drawFormField(int field, const std::string& label, int row);
	int drawFormSelector(const std::string& label, const std::string& value, int row);
	bool drawFormButton(const std::string& label);
	void drawMainMenu();
	void drawFileMenu();
	void drawAbout();
	void drawMaintenanceMenu();
	void drawMovieMaintenance();
	void drawRoomMaintenance();
	void drawScheduleMaintenance();
	void drawScheduleForm();
	void drawReservationMovies();
	void drawMovieCard(int index, const sf::FloatRect& area);
	void drawReservationRooms();
	void drawReservationSchedules();
	void drawScheduleRow(int index, const sf::FloatRect& area);
	void drawReservationSeats();
	void drawSeatGrid(const movieTheater& theater);
	void drawSeat(const Seats& seat, int number, const sf::FloatRect& area);
	void drawSeatLegend(float y);
	void drawReservationSummary(const movieTheater& theater);
	void drawReservationTicket();
	void drawSale();
	void drawSaleDetails(const sf::FloatRect& area);
	void drawCustomerForm(const sf::FloatRect& area);
	void drawPaidSale(const Customer& customer, float x, float y);
	void saveMovie();
	void saveRoom();
	void saveSchedule();
	void confirmReservation();
	void searchReservation();
	void payReservation();
	bool hasRoomNumber(int number) const;
	bool canSellVoucher(int voucher) const;
	std::vector<int> findRoomNumbers(int movie) const;
	std::vector<int> findTheaters(int movie, int room) const;
	int countUpcoming(const std::vector<int>& theaterIndexes) const;
	int countSelectedSeats() const;
	std::vector<int> collectSelectedSeats() const;
	sf::Color chooseSeatColor(const Seats& seat, bool isSelected, bool isHovered) const;
	std::pair<std::string, sf::Color> describeSaleStatus(int voucher) const;
	std::string describeMovie(const Movies& movie) const;
	std::string describeRoom(int seatCount, int price) const;
	std::string describeSchedule(const movieTheater& theater) const;
	std::string formatScheduleTime(const movieTheater& theater) const;
	static std::string joinSeats(const std::vector<int>& seats);
	static std::string formatCount(int count, const std::string& singular, const std::string& plural);
	static int toNumber(const std::string& value);
	static void removeLastCharacter(std::string& value);
};
