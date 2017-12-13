#include "sprite2/TextTable.h"

#include <fstream>

namespace s2
{

CU_SINGLETON_DEFINITION(TextTable)

TextTable::TextTable()
{
}

void TextTable::Insert(const CU_STR& tid, const CU_VEC<CU_STR>& languages)
{
	m_texts.insert(std::make_pair(tid, languages));
}

CU_STR TextTable::Query(LanguageIdx lang, const CU_STR& tid) const
{
	if (lang < 0 || lang >= LANG_COUNT) {
		return "";
	}

	auto itr = m_texts.find(tid);
	if (itr == m_texts.end()) {
		return "";
	}
	if (lang < 0 || lang >= itr->second.size()) {
		return "";
	}

	return itr->second[lang];
}

void TextTable::Clear()
{
	m_texts.clear();
}

}