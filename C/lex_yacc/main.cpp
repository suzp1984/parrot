#include "main.hpp"

void Include::clear()
{
	headerfile.clear();
	is_quotation = false;
	is_angle = false;
}

std::ostream &operator <<(std::ostream &s, const Include &I)
{
	if(I.is_angle)
		s << "user <";

	if(I.is_quotation)
		s << "user O";

	s << "headerfile: [" << I.headerfile << "]";
	return s;
}
