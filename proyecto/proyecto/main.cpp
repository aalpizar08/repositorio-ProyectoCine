#include <ctime>
#include <vector>
#include "gui.h"
#include "movieTheater.h"
#include "movies.h"
#include "room.h"
using namespace std;

time_t roundUp(time_t value) {
	return value - value % 300 + 300;
}
time_t createTomorrowTime(time_t now, int hour, int minute) {
	tm parts = {};
	localtime_s(&parts, &now);
	parts.tm_mday += 1;
	parts.tm_hour = hour;
	parts.tm_min = minute;
	parts.tm_sec = 0;
	parts.tm_isdst = -1;
	return mktime(&parts);
}
movieTheater createTheater(const Movies& movie, const Room& room, time_t start) {
	time_t end = start + (movie.getDuration() + 15) * 60;
	return movieTheater(movie.getName(), movie.getYear(), movie.getDuration(), movie.getCountry(), movie.getReview(), room.getNumber(), room.getPrice(), room.getSeatCount(), start, end);
}
int main() {
	time_t now = time(nullptr);
	vector<Movies> movies = {
		Movies("Deadpool & Wolverine", 2024, 127, "Estados Unidos", "3,6/5"),
		Movies("El niño y la garza", 2023, 124, "Japón", "4/5"),
		Movies("La vida es bella", 1997, 116, "Italia", "4,3/5"),
		Movies("Wallace y Gromit: la batalla de los vegetales", 2005, 85, "Reino Unido", "3,8/5")
	};
	vector<Room> rooms = { Room(1, 25, 4000), Room(2, 25, 3000) };
	vector<movieTheater> theaters = {
		createTheater(movies[0], rooms[0], roundUp(now + 20 * 60)),
		createTheater(movies[0], rooms[0], roundUp(now + 180 * 60)),
		createTheater(movies[1], rooms[0], createTomorrowTime(now, 15, 0)),
		createTheater(movies[2], rooms[1], roundUp(now + 90 * 60)),
		createTheater(movies[3], rooms[1], createTomorrowTime(now, 17, 0)),
		createTheater(movies[2], rooms[1], createTomorrowTime(now, 20, 0))
	};
	Gui gui(movies, rooms, theaters);
	if (!gui.loadFont("C:/Windows/Fonts/arial.ttf")) {
		return 1;
	}
	gui.run();
	return 0;
}
