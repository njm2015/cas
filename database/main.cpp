#include <iostream>
#include <vector>

#include "Date.h"
#include "write.h"

void usage() {
	std::cout << "./database <filename> <destination>" << std::endl;
}

int main(int argc, char **argv) {

	Date d("2017-11-1");

	std::cout << d.get_date_URL() << std::endl;

	return 0;
}