#include <iostream>
#include <string>
#include <curl/curl.h>
#include <limits>
#include <list>
#include <algorithm>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

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

	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	year -= month < 3;
	return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

long seconds_since_epoch(int day, int month, int year) {

	if(year < 70) {
		error("parsing date. please enter date after 1970");
		return -1;
	}

	long year_day, mon_day;

	year_day = (year - 70) * 365 + (year-70) / 4 + 1;

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

	return (year_day + mon_day + day) * 86400;	
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