#include <iostream>
#include <string>
#include <curl/curl.h>
#include <limits>
#include <list>
#include <vector>
#include <algorithm>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include "parser.h"

void usage(std::string func_name) {

	std::cout << "Incorrect usage for " << func_name << ". 'help' for usages." << std::endl;
}

void error(std::string message) {

	std::cout << "Error in " << message << std::endl;
}

void unknown_func() {

	std::cout << "enter known function (list of functions 'help')" << std::endl;
}


int day_of_week(int day, int month, int year) {
	/* see .h file for algo credit */
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	year -= month < 3;
	return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

long days_since_epoch(int day, int month, int year) {

	if(year < 70) {
		error("parsing date. please enter date after 1970");
		return -1;
	}

	long year_day, mon_day;

	year_day = (year - 70) * 365 + (year-72) / 4 + 1;

	mon_day = 0;

	std::list<int> thirty = {3, 5, 8, 10};
	std::list<int> thirty_one = {0, 2, 4, 6, 7, 9, 11};

	for(int i = 0; i < month; i++) {
		if(std::find(thirty_one.begin(), thirty_one.end(), i) != thirty_one.end()) {
			mon_day += 31;
		} else if(std::find(thirty.begin(), thirty.end(), i) != thirty.end()) {
			mon_day += 30;
		} else {
			mon_day += 28;
		}
	}

	return year_day + mon_day + day;	
}

tm* get_nearest_friday(tm* date) {

	tm* ret = new tm();
	int day;

	ret->tm_mday = date->tm_mday;
	ret->tm_mon = date->tm_mon;
	ret->tm_year = date->tm_year;

	day = day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);

	if(day == 5) {
		return ret;
	}

	if(day == 6) {
		ret->tm_mday = date->tm_mday + 6;
	} else {
		ret->tm_mday = date->tm_mday + 5 - day;
	}

	std::list<int> thirty = {3, 5, 8, 10};
	std::list<int> thirty_one = {0, 2, 4, 6, 7, 9, 11};

	if(std::find(thirty.begin(), thirty.end(), date->tm_mon) != thirty.end() && ret->tm_mday > 30) {
		ret->tm_mon++;
		ret->tm_mday -= 30;
	} else if(std::find(thirty_one.begin(), thirty_one.end(), ret->tm_mon) != thirty_one.end() && ret->tm_mday > 31) {
		ret->tm_mon++;
		ret->tm_mday -= 31;
	} else if(ret->tm_mon == 1 && ret->tm_mday > 28) {
		ret->tm_mon++;
		ret->tm_mday -= 28;
	}

	if(ret->tm_mon > 11) {
		ret->tm_year++;
		ret->tm_mon -= 12;
	}

	return ret;
}

int days_in_month(int month) {

	std::list<int> thirty = {3, 5, 8, 10};
	std::list<int> thirty_one = {0, 2, 4, 6, 7, 9, 11};

	if(std::find(thirty.begin(), thirty.end(), month) != thirty.end()) {
		return 30;
	}

	if(std::find(thirty_one.begin(), thirty_one.end(), month) != thirty_one.end()) {
		return 31;
	}

	return 28;
}

bool date_agree(int day, int month) {

	return (day <= days_in_month(month));
}

int add_to_date_no_weekends(tm* date, int day) {

	int weekend = day / 5;
	int weekend_mod = day % 5;
	if(weekend_mod + day_of_week(date->tm_mday, date->tm_mon + 1, date->tm_year + 1900) > 5) {
		weekend++;
	}

	date->tm_mday += (day + (weekend * 2));
	while(date->tm_mday > days_in_month(date->tm_mon)) {
		date->tm_mday -= days_in_month(date->tm_mon);
		date->tm_mon++;
		
		if(date->tm_mon > 11) {
			date->tm_year++;
			date->tm_mon = 0;
		}
	}

	return weekend * 2;
}

long get_opt_seconds(tm* date) {

	return (days_since_epoch(date->tm_mday, date->tm_mon, date->tm_year) - 1) * 86400;
}

std::string date_to_string(tm* date) {

	std::string month;

	switch(date->tm_mon) {
		case 0:
			month = "Jan";
			break;
		case 1:
			month = "Feb";
			break;
		case 2:
			month = "Mar";
			break;
		case 3:
			month = "Apr";
			break;
		case 4:
			month = "May";
			break;
		case 5:
			month = "Jun";
			break;
		case 6:
			month = "Jul";
			break;
		case 7:
			month = "Aug";
			break;
		case 8:
			month = "Sept";
			break;
		case 9:
			month = "Oct";
			break;
		case 10:
			month = "Nov";
			break;
		case 11:
			month = "Dec";
			break;
	}

	return month + " " + std::to_string(date->tm_mday) + ", " + std::to_string(date->tm_year + 1900);
}

std::string modify_date_format(std::string date) {

	static std::map<std::string, int> mapMonth;

	mapMonth["jan"] = 1;
	mapMonth["feb"] = 2;
	mapMonth["mar"] = 3;
	mapMonth["apr"] = 4;
	mapMonth["may"] = 5;
	mapMonth["jun"] = 6;
	mapMonth["jul"] = 7;
	mapMonth["aug"] = 8;
	mapMonth["sep"] = 9;
	mapMonth["oct"] = 10;
	mapMonth["nov"] = 11;
	mapMonth["dec"] = 12;

	std::transform(date.begin(), date.end(), date.begin(), ::tolower);
	std::vector<std::string> date_list;

	for(int i = 0; i < 2; i++) {
		size_t space = date.find(" ");
		date_list.push_back(date.substr(0,space));
		date = date.substr(space+1);
	}
	date_list.push_back(date);

	int day, year;
	day = std::stoi(date_list[1]);
	year = std::stoi(date_list[2]);

	std::string ret = std::to_string(mapMonth[date_list[0]]) + std::string("/")
					+ std::to_string(day) + std::string("/")
					+ std::to_string(year);

	return ret;
}

tm* string_to_date(std::vector<std::string> dates) {

	tm* ret = new tm();

	static std::map<std::string, int> mapMonth;

	mapMonth["january"] = 0;
	mapMonth["february"] = 1;
	mapMonth["march"] = 2;
	mapMonth["april"] = 3;
	mapMonth["may"] = 4;
	mapMonth["june"] = 5;
	mapMonth["july"] = 6;
	mapMonth["august"] = 7;
	mapMonth["september"] = 8;
	mapMonth["october"] = 9;
	mapMonth["november"] = 10;
	mapMonth["december"] = 11;

	std::transform(dates[0].begin(), dates[0].end(), dates[0].begin(), ::tolower);

	ret->tm_mon = mapMonth[dates[0]];
	ret->tm_mday = std::stoi(dates[1]);
	ret->tm_year = std::stoi(dates[2]) - 1900;

	return ret;
}

tm* closest_date(tm* date, std::vector<tm*> date_list) {

	int last = 0;

	for(int i = 0; i < date_list.size(); i++) {
		if(date_compare(date, date_list[i], false)) {
			break;
		} else {
			last = i;
		}
	}

	return date_list[last];
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string get_page(std::string URL) {

	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();

	const char* new_URL = URL.c_str();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, new_URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		return readBuffer;
	} else {
		error("in getting URL");

		std::string ret = "error";
		return ret;
	}
}

xmlDoc* get_tree(std::string html_page) {

	return htmlReadDoc((xmlChar*)html_page.c_str(), NULL, NULL,
						HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
}

xmlNode* get_root(xmlDoc* tree) {

	return xmlDocGetRootElement(tree);
}

std::vector<std::string> get_xml_element_text(std::string URL, std::string xpath) {

	std::vector<std::string> ret;

	std::string html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		ret.push_back(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content());
		delete elements[0];
	} else {
		std::cout << "Invalid symbol" << std::endl;
	}

	delete root;
	xmlFreeDoc(doc);

	return ret;
}