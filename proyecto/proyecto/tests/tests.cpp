#include <iostream>
#include <string>
#include <vector>
#include "../archive.h"
#include "../booking.h"
#include "../dateTime.h"
#include "../movieTheater.h"
#include "../room.h"
#include "../sales.h"

namespace {
	int failures = 0;
	int checks = 0;
	const time_t hour = 60 * 60;
	const time_t minute = 60;
	const std::string validId = "112340567";
	const std::string validCard = "4111111111111111";

	void check(bool condition, int line) {
		checks++;
		if (!condition) {
			failures++;
			std::cout << "  FALLO linea " << line << std::endl;
		}
	}

	movieTheater createTheater(int roomNumber, int seatCount, time_t start) {
		return movieTheater("Deadpool & Wolverine", 2024, 127, "Estados Unidos", "3,6/5", roomNumber, 4000, seatCount, start, start + 2 * hour);
	}

	void testSeats() {
		Seats seat;
		check(!seat.isEnabled() && !seat.isAvailable() && !seat.isSold(), __LINE__);
		seat.enable();
		seat.setNumber(7);
		check(seat.isAvailable(), __LINE__);
		seat.setNumber(-3);
		check(seat.isReserved() && !seat.isAvailable(), __LINE__);
		seat.setNumber(0);
		check(seat.isSold() && !seat.isReserved(), __LINE__);
	}

	void testMovieTheaterSeats() {
		movieTheater theater = createTheater(1, 25, 100 * hour);
		check(theater.freeSeats() == 25, __LINE__);
		check(theater.getSeat(4, 4).getNumber() == 25, __LINE__);
		check(theater.disponibility(1) && theater.disponibility(25), __LINE__);
		check(!theater.disponibility(0) && !theater.disponibility(-1) && !theater.disponibility(26), __LINE__);
		theater.setReserved(5, 1);
		check(!theater.disponibility(5), __LINE__);
		check(theater.getSeat(0, 4).isReserved(), __LINE__);
		check(theater.freeSeats() == 24, __LINE__);
		theater.setReserved(5, 2);
		check(theater.getSeat(0, 4).getNumber() == -1, __LINE__);
		check(!theater.disponibility(0), __LINE__);
		theater.sold(1);
		check(theater.getSeat(0, 4).isSold(), __LINE__);
		check(!theater.disponibility(0), __LINE__);
		for (int seat = 1; seat <= 25; seat++) {
			theater.setReserved(seat, 3);
		}
		check(theater.fullRoom(), __LINE__);
	}

	void testMovieTheaterSeatCount() {
		movieTheater theater = createTheater(2, 12, 100 * hour);
		check(theater.getSeatCount() == 12, __LINE__);
		check(theater.freeSeats() == 12, __LINE__);
		check(theater.disponibility(12) && !theater.disponibility(13), __LINE__);
		check(!theater.getSeat(2, 2).isEnabled(), __LINE__);
		check(!theater.getSeat(2, 2).isSold(), __LINE__);
	}

	void testMovieTheaterTimes() {
		time_t start = 100 * hour;
		movieTheater theater = createTheater(1, 25, start);
		check(theater.canSell(start - 31 * minute), __LINE__);
		check(!theater.canSell(start - 30 * minute), __LINE__);
		check(!theater.canSell(start - 10 * minute), __LINE__);
		check(!theater.hasStarted(start - 1) && theater.hasStarted(start), __LINE__);
		check(theater.overlaps(createTheater(1, 25, start + hour)), __LINE__);
		check(!theater.overlaps(createTheater(1, 25, start + 2 * hour)), __LINE__);
		check(!theater.overlaps(createTheater(2, 25, start)), __LINE__);
	}

	void testBooking() {
		Booking book;
		check(!book.detectVoucher(1), __LINE__);
		int first = book.toBooking(3, 2, 8000);
		int second = book.toBooking(1, 1, 3000);
		check(first == 1 && second == 2, __LINE__);
		check(book.detectVoucher(first) && book.detectVoucher(second), __LINE__);
		check(!book.detectVoucher(0) && !book.detectVoucher(99), __LINE__);
		check(book.getTheater(first) == 3 && book.getTickets(first) == 2 && book.getTotal(first) == 8000, __LINE__);
		check(book.getTotal(second) == 3000, __LINE__);
		check(!book.isPaid(first), __LINE__);
		book.setPaid(first);
		check(book.isPaid(first) && !book.isPaid(second), __LINE__);
	}

	void testSales() {
		Sales sale;
		check(sale.isValidCustomer(validId, validCard), __LINE__);
		check(sale.isValidCustomer("123456789012", validCard), __LINE__);
		check(!sale.isValidCustomer("12345678", validCard), __LINE__);
		check(!sale.isValidCustomer(validId, "411111111111111"), __LINE__);
		check(!sale.isValidCustomer(validId, "4111a11111111111"), __LINE__);
		check(!sale.pay(1, "1", "2"), __LINE__);
		check(!sale.detectSale(1), __LINE__);
		check(sale.pay(1, validId, validCard), __LINE__);
		check(sale.detectSale(1) && !sale.detectSale(2), __LINE__);
		check(!sale.pay(1, validId, validCard), __LINE__);
		check(sale.getCustomer(1).getID() == validId, __LINE__);
		check(sale.getCustomer(1).getMaskedCard() == "**** **** **** 1111", __LINE__);
		check(sale.pay(2, "123456789", "5555555555554444"), __LINE__);
		check(sale.getCustomer(2).getID() == "123456789", __LINE__);
	}

	void testRoom() {
		Room room(3, 20, 2500);
		check(room.getNumber() == 3 && room.getSeatCount() == 20 && room.getPrice() == 2500, __LINE__);
	}

	void testDateTime() {
		time_t value = 0;
		check(DateTime::parse("26/10/2026", "15:30", value), __LINE__);
		check(DateTime::formatDate(value) == "26/10/2026", __LINE__);
		check(DateTime::formatTime(value) == "15:30", __LINE__);
		check(!DateTime::parse("31/02/2026", "15:30", value), __LINE__);
		check(!DateTime::parse("26/10/2026", "24:00", value), __LINE__);
		check(!DateTime::parse("26-10-2026", "15:30", value), __LINE__);
	}

	void testArchive() {
		Archive archive;
		check(archive.getInfo().find("Alejandro Rafael Alpizar Rodriguez") != std::string::npos, __LINE__);
	}
}

int main() {
	struct Test {
		const char* name;
		void (*run)();
	};
	std::vector<Test> tests = {
		{ "Seats", testSeats },
		{ "movieTheater: reservar y vender butacas", testMovieTheaterSeats },
		{ "movieTheater: cantidad de butacas de la sala", testMovieTheaterSeatCount },
		{ "movieTheater: regla de 30 minutos y traslapes", testMovieTheaterTimes },
		{ "Booking: consecutivos", testBooking },
		{ "Sales: cedula, tarjeta y pago", testSales },
		{ "Room", testRoom },
		{ "DateTime", testDateTime },
		{ "Archive", testArchive },
	};
	for (const Test& test : tests) {
		int before = failures;
		test.run();
		std::cout << (failures == before ? "[ OK ] " : "[FALLA] ") << test.name << std::endl;
	}
	std::cout << std::endl << checks - failures << "/" << checks << " verificaciones correctas" << std::endl;
	return failures == 0 ? 0 : 1;
}
