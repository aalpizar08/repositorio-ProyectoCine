#include "booking.h"
#include "seats.h"
void Booking::toBooking(int voucher, int totalToPay) {
	for (int i = 0; i < ROWSBOOKING; i++) {
		if (vouchers[i][0] == 0) {
			vouchers[i][0] = voucher;
			vouchers[i][1] = totalToPay;
			
		}
	}
	cout << "Reserva hecha, el numero de boucher es de " << voucher << endl;
	cout << "y el monto a pagar es de " << totalToPay << endl;
	system("PAUSE");
}

bool Booking::detectVoucher(int voucher)
{
	for (int i = 0; i < ROWSBOOKING; i++) {
		for (int j = 0; j < COLUMNSBOOKING; j++) {
			if (vouchers[i][j]) {
				return true;
			}
		}
	}
	return false;
}

