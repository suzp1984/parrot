#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

struct Include
{
	void clear();
	std::string headerfile;
	bool is_quotation;
	bool is_angle;
	friend std::ostream &operator<<(std::ostream &s, const Include &I);
};

std::ostream&operator<<(std::ostream &s, const Include &I);

#define YYSTYPE Include

