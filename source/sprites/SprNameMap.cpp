#include "SprNameMap.h"

namespace s2
{

SINGLETON_DEFINITION(SprNameMap);

SprNameMap::SprNameMap() 
	: m_next_static_id(0)
	, m_next_tmp_id(MAX_STATIC_ID)
	, m_next_normal_id(MAX_TMP_ID)
{
	Insert("base", BASE_ID);
	Insert("mask", MASK_ID);
	m_next_static_id = MASK_ID + 1;
}

int SprNameMap::StrToID(const std::string& str)
{
	std::map<std::string, int>::iterator itr 
		= m_str2id.find(str);
	if (itr != m_str2id.end()) {
		return itr->second;
	} else {
		return Insert(str);
	}
}

bool SprNameMap::IDToStr(int id, std::string& str)
{
	std::map<int, std::string>::iterator itr 
		= m_id2str.find(id);
	if (itr != m_id2str.end()) {
		str = itr->second;
		return true;
	} else {
		return false;
	}
}

int SprNameMap::QueryID(const std::string& str)
{
	std::map<std::string, int>::iterator itr 
		= m_str2id.find(str);
	if (itr != m_str2id.end()) {
		return itr->second;
	} else {
		return -1;
	}
}

bool SprNameMap::IsTmpName(int id)
{
	return id >= MAX_STATIC_ID && id < MAX_TMP_ID;
}

int SprNameMap::Insert(const std::string& str)
{
	if (str.empty()) {
		return -1;
	} else {
		return str[0] == '_' ? Insert(str, NAME_TMP) : Insert(str, NAME_NORMAL);
	}
}

int SprNameMap::Insert(const std::string& str, NameType type)
{
	int id = -1;
	switch (type)
	{
	case NAME_STATIC:
		id = m_next_static_id++;
		break;
	case NAME_TMP:
		id = m_next_tmp_id++;
		break;
	case NAME_NORMAL:
		id = m_next_normal_id++;
		break;
	default:
		return -1;
	}

	Insert(str, id);

	return id;
}

void SprNameMap::Insert(const std::string& str, int id)
{
	m_str2id.insert(std::make_pair(str, id));
	m_id2str.insert(std::make_pair(id, str));
}

}