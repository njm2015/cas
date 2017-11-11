#ifndef DATE_H
#define DATE_H

#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <algorithm>

class Date {

public:

	Date();

	Date(std::string str);

	int get_day();
	int get_month();
	int get_year();
	int get_hour();
	int get_minute();
	int get_days_in_month();

	void set_day(int day);
	void set_month(int month);
	void set_year(int year);
	void set_hour(int hour);
	void set_minute(int minute);

	int fix_month();
	int fix_hour();
	int fix_day();
	int fix_minute();

	void add_date(int minutes);

	std::string get_date_URL();
	std::string get_date_full();

private:
	int minute, hour, day, month, year;

};

#endif