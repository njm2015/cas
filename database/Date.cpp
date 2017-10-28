#include "Date.h"

Date::Date() : month(0), day(0), year(0) {}

Date::Date(std::string str) {
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

void Date::print_date() {
	std::cout << "Day: " << this->day << "\tMonth: " << this->month <<
				"\tYear: " << this->year << std::endl;
}