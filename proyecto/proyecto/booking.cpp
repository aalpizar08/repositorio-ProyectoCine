#include "booking.h"
int Booking::toBooking(int theater, int tickets, int totalToPay) {
	for (int i = 0; i < ROWSBOOKING; i++) {
		if (vouchers[i][VOUCHERCOLUMN] == 0) {
			vouchers[i][VOUCHERCOLUMN] = nextVoucher;
			vouchers[i][TOTALCOLUMN] = totalToPay;
			vouchers[i][THEATERCOLUMN] = theater;
			vouchers[i][TICKETSCOLUMN] = tickets;
			vouchers[i][PAIDCOLUMN] = 0;
			nextVoucher++;
			return vouchers[i][VOUCHERCOLUMN];
		}
	}
	return 0;
}

bool Booking::detectVoucher(int voucher) const
{
	return findRow(voucher) >= 0;
}
int Booking::getTheater(int voucher) const {
	return vouchers[findRow(voucher)][THEATERCOLUMN];
}
int Booking::getTickets(int voucher) const {
	return vouchers[findRow(voucher)][TICKETSCOLUMN];
}
int Booking::getTotal(int voucher) const {
	return vouchers[findRow(voucher)][TOTALCOLUMN];
}
bool Booking::isPaid(int voucher) const {
	return vouchers[findRow(voucher)][PAIDCOLUMN] == 1;
}
void Booking::setPaid(int voucher) {
	vouchers[findRow(voucher)][PAIDCOLUMN] = 1;
}
int Booking::findRow(int voucher) const {
	if (voucher <= 0) {
		return -1;
	}
	for (int i = 0; i < ROWSBOOKING; i++) {
		if (vouchers[i][VOUCHERCOLUMN] == voucher) {
			return i;
		}
	}
	return -1;
}
