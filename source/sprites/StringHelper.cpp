#include "sprite2/StringHelper.h"

namespace s2
{

CU_STR StringHelper::FromChar(const char* c_str)
{
	CU_STR str;
	if (c_str) {
		str.assign(c_str);
	}
	return str;
}

}