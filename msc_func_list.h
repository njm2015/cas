#ifndef MSC_FUNC_LIST_H
#define MSC_FUNC_LIST_H

#include <libxml/tree.h>

/**
* Basic usage function that defers the user
* to the help menu
* @param func_name The name of the function to be displayed
**/

void usage(std::string func_name);

/**
* Called if an error happens and displays a custom message
* to the user.
* @param message The message to be displayed to the user
* following "Error in"
**/

void error(std::string message);

/**
* Called if user enters unknown command. Then prompts user
* to open help menu.
**/

void unknown_func();

/**
* Algotrithm pulled from GeeksforGeeks that produces the day
* of the week (Sunday, Monday, etc) given the parameters.
* @param day The day of the month
* @param month The month (1-12)
* @param year The actual year
* @return Value 0-6 for Sunday-Saturday
**/

int day_of_week(int day, int month, int year);

/**
* Calculates the number of seconds since epoch and given
* date.
* @param day integer value of day
* @param month integer value of month TM FORMAT (0-11)
* @param year integer value of year TM FORMAT (year - 1900)
* @return The number of seconds since Jan 1, 1970 to date
**/

long days_since_epoch(int day, int month, int year);

/**
* Calculates the first friday on or after the given date.
* @param date The date given for reference
* @return the closest Friday on or after the reference date
**/

tm* get_nearest_friday(tm* date);

/**
* Calculates the number of days in the reference month
* @param month The integer value of the month TM FORMAT (0-11)
* @return The integer value of the number of days in reference month
**/

int days_in_month(int month);

/**
* Calulates if the number of days in the reference month
* agrees with the reference days.
* @param day The integer value of the day
* @param month The integer value of the month TM FORMAT (0-11)
* @return true if reference month can have reference day,
* false otherwise
**/

bool date_agree(int day, int month);

/**
* Adds a given number of days to a certain date and calculates
* correct months and years accordingly
* @param date The reference date to add days to
* @param day Number of days to add to date
* @return A pointer to a new tm variable on the heap with the
* added days
**/

int add_to_date_no_weekends(tm* date, int day);

/**
* Special function only used for constructing the URL for the option
* call/put functions
**/

long get_opt_seconds(tm* date);

std::string modify_date_format(std::string date);

/**
* Converts date into a readable format
* @param date The date to be converted
* @return String containing readable date
**/

std::string date_to_string(tm* date);

tm* string_to_date(std::vector<std::string> dates);

tm* closest_date(tm* date, std::vector<tm*> date_list);

/**
* Function to write URL to string instead of cout
* Has same params as default WriteCallback function
**/

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

/**
* Helper function to get html page
* @param URL The URL to retrieve
* @return The html page in string format
**/

std::string get_page(std::string URL);

/**
* Creates a xml tree from the html page
* @param html_page The page to make tree from
* @return A pointer to the xml tree
**/

xmlDoc* get_tree(std::string html_page);

/**
* Gets the root of a reference xml tree
* @param tree The tree to retrieve root from
* @return A pointer to the xml node that is the root
**/

xmlNode* get_root(xmlDoc* tree);

/**
* Helper function that bypasses low level elements of the CURL
* and libxml++ libraries
* @param URL The URL of the page to retrieve
* @param xpath The XPath expression of the xml node to retrieve
* @return The string representation of the retrieved xml node's text
**/

std::vector<std::string> get_xml_element_text(std::string URL, std::string xpath);

#endif