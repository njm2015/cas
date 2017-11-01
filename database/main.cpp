#include <iostream>
#include <vector>

#include "Date.h"

void usage() {
	std::cout << "./database <filename> <destination>" << std::endl;
}

int main(int argc, char **argv) {

	if(argc != 3) {
		usage();
		exit(1);
	}

	Date *d = new Date("10/13/2009");

	std::cout << "Minute: " << d->get_minute() << "\tHour: " << d->get_hour() << 
				"\tDay: " << d->get_day() << "\tMonth: " << d->get_month() <<
				"\tYear: " << d->get_year() << std::endl;

	delete(d);

	return 0;
}