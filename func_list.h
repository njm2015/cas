#ifndef FUNCTION_LIST_H
#define FUNCTION_LIST_H

#include <iostream>
#include <string>
#include <curl/curl.h>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

void usage(std::string func_name);

int day_of_week(int day, int month, int year);

void error(std::string message);

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

std::string get_page(std::string URL);

void unknown_func();

double get_price(std::vector<std::string> args);

#endif