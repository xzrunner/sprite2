#include "sprite2/StringHelper.h"

namespace s2
{

std::string StringHelper::FromChar(const char* c_str)
{
	std::string str;
	if (c_str) {
		str.assign(c_str);
	}
	return str;
}

}