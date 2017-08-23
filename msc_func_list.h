#ifndef MSC_FUNC_LIST_H
#define MSC_FUNC_LIST_H

#include <libxml/tree.h>

void usage(std::string func_name);

void error(std::string message);

void unknown_func();

int day_of_week(int day, int month, int year);

long seconds_since_epoch(int day, int month, int year);

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

std::string get_page(std::string URL);

xmlDoc* get_tree(std::string html_page);

xmlNode* get_root(xmlDoc* tree);

std::vector<std::string> get_xml_element_text(std::string URL, std::string xpath);

#endif