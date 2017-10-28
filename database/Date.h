#ifndef DATE_H
#define DATE_H

#include <vector>
#include <iostream>

class Date {

public:

	Date();

	Date(std::string str);

	int get_day();
	int get_month();
	int get_year();
	int get_hour();
	int get_minute();

	void set_day(int day);
	void set_month(int month);
	void set_year(int year);
	void set_hour(int hour);
	void set_minute(int minute);

	void add_date(int minutes);

	void print_date();

private:
	int month; // January = 1, ...
	int day; // 1-31
	int year;
	int hour;
	int minute;
};

#endif