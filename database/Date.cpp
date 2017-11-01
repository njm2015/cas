#include "Date.h"

static const std::list<int> thirty = {4, 6, 9, 11};
static const std::list<int> thirty_one = {1, 3, 5, 7, 8, 10, 12};

Date::Date() : minute(0), hour(0), day(0), month(0), year(0) {}

Date::Date(std::string str) : minute(0), hour(0) {
	std::vector<int> date_args;
	size_t found, start_i = 0;

	for(size_t i = 0; i < 2; ++i) {
		found = str.find("/");
		date_args.push_back(std::stoi(str.substr(start_i, found - start_i)));
		str = str.substr(found+1);
	}
	date_args.push_back(std::stoi(str));

	this->month = date_args[0];
	this->day = date_args[1];
	this->year = date_args[2];
}

int Date::get_day() {
	return this->day;
}

int Date::get_month() {
	return this->month;
}

int Date::get_year() {
	return this->year;
}

int Date::get_hour() {
	return this->hour;
}

int Date::get_minute() {
	return this->minute;
}

int Date::get_days_in_month() {
	if(std::find(thirty_one.begin(), thirty_one.end(), this->month) 
													!= thirty_one.end()) {
		return 31;
	} else if(this->month == 2) {
		return 28;
	} else {
		return 30;
	}
}

void Date::set_day(int day) {
	if(day <= 0 || day > 31) {
		return;
	}
	this->day = day;
}

void Date::set_month(int month) {
	if(month <= 0 || month > 12) {
		return;
	}
	this->month = month;
}

void Date::set_year(int year) {
	if(year < 1970) {
		return;
	}
	this->year = year;
}

void Date::set_hour(int hour) {
	if(hour < 0 || hour > 23) {
		return;
	}
	this->hour = hour;
}

void Date::set_minute(int minute) {
	if(minute < 0 || minute > 59) {
		return;
	}
	this->minute = minute;
}

void Date::fix_month() {
	if(m > 11) {
		int m = this->month - 1;
		int y = m / 12;
		m = m % y;
		this->year += y;
		this->month = m + 1;

		return 1;
	}
	return 0;
}

void Date::fix_day() {
	int ret_val = 0;
	while(this->day > this->get_days_in_month()) {
		this->day -= this->get_days_in_month();
		++this->month;
		this->fix_month();
		ret_val = 1;
	}
	return ret_val;
}

void Date::fix_hour() {
	if(this->hour > 23) {
		int d = this->hour / 24;
		this->hour = this->hour % d;
		this->day += d;
		return 1;
	}
	return 0;
}

void Date::fix_minute() {
	if(this->minute > 60) {
		int h = this->minute / 60;
		this->minute = this->minute % h;
		this->hour += h;
		return 1;
	}
	return 0;
}

void Date::add_date(int minutes) {
	this->minute += minutes;
	if(this->fix_minute()) {
		if(this->fix_hour()) {
			if(this->fix_day()) {
				this->fix_month();
			}
		}
	}
}

void Date::print_date() {
	std::cout << "Day: " << this->day << "\tMonth: " << this->month <<
				"\tYear: " << this->year << std::endl;
}