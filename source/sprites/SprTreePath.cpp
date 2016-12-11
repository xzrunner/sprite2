#include "SprTreePath.h"

#include <iostream>

namespace s2
{

SprTreePath::SprTreePath()
	: m_val(0)
{
}

bool SprTreePath::operator == (const SprTreePath& path) const
{
	if (path.m_val != m_val || path.m_ids.size() != m_ids.size()) {
		return false;
	}

	for (int i = 0, n = m_ids.size(); i < n; ++i) {
		if (m_ids[i] != path.m_ids[i]) {
			return false;
		}
	}

	return true;
}

std::ostream & operator << (std::ostream& os, const SprTreePath& path)
{
	os << "[path: ";
	for (int i = 0, n = path.m_ids.size(); i < n; ++i) {
		os << path.m_ids[i] << ", ";
	}
	os << "]";
	return os;
}

}