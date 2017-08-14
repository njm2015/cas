#include "func_list.h"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <algorithm>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

void usage(std::string func_name) {

	std::cout << "Incorrect usage for " << func_name << ". 'help' for usages." << std::endl;
}

int day_of_week(tm* date) {

	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	date->tm_year -= date->tm_mon < 3;
	return (date->tm_year + date->tm_year/4 - date->tm_year/100 + date->tm_year/400 + t[date->tm_mon-1] + date->tm_mday) % 7;
}

void error(std::string message) {

	std::cout << "Error in " << message << std::endl;
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

void unknown_func() {

	std::cout << "enter known function (list of functions 'help')" << std::endl;
}

double get_price(std::string symbol, std::vector<std::string> args) {
	
	double price;
	std::string URL, html, xpath;

	if(args.size() < 1) {
		usage("getPrice");
		return -1;
	}
/*
	tm* date = parse_date(args[0]);

	if(!date)
		return -1;

	if(dayOfWeek)

	URL = "http://bigcharts.marketwatch.com/historical/default.asp?symb="
						+ symbol + "&closeDate=" + date->tm_mon 
						+ "%2F" + date->tm_mday + "%2F" + date->tm_year;
*/
	html = get_page(URL);

	xmlDoc* doc = get_tree(html);
	xmlpp::Element* root = new xmlpp::Element(get_root(doc));

	xpath = "//table[@class=\"historicalquote fatbottomed\"]//tr[3]/td/text()";

	auto elements = root->find(xpath);
	if(elements.size() == 1) {
		price = std::stod(dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content(), nullptr);
	} else {
		std::cout << "Invalid symbol" << std::endl;
		return -1;
	}

	delete root;
	//delete date;
	xmlFreeDoc(doc);

	return price;	
}