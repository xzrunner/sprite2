#include "sprite2/TextTable.h"

#ifdef S2_TEXT_TABLE_DB
#include <bimp/BIMP_ImportStream.h>
#include <playdb/storage/DiskStorageManager.h>
#endif // S2_TEXT_TABLE_DB

#include <fstream>

namespace s2
{

CU_SINGLETON_DEFINITION(TextTable)

TextTable::TextTable()
{
}

int TextTable::QueryType(const CU_STR& lang) const
{
	for (size_t i = 0, n = m_types.size(); i < n; ++i) {
		if (m_types[i] == lang) {
			return i;
		}
	}
	return -1;
}

#ifdef S2_TEXT_TABLE_DB

CU_STR TextTable::Query(int lang, const CU_STR& tid) const
{
	playdb::btree::Data<CU_STR> data;
	if (!m_tree->Query(tid, data)) {
		return "";
	}

	bimp::ImportStream is((const char*)(data.data), data.data_len);
	uint8_t num = is.UInt8();
	std::vector<CU_STR> langs;
	langs.reserve(num);
	for (int i = 0; i < num; ++i) {
		langs.push_back(is.LongString());
	}
	return langs[lang];
}

void TextTable::LoadDB(const CU_STR& index_path, const CU_STR& data_path)
{
	m_storage_mgr = std::make_unique<playdb::storage::DiskStorageManager>(
		index_path, data_path);
	m_tree = std::make_unique<playdb::btree::BTree<CU_STR>>(m_storage_mgr.get());
}

#else

CU_STR TextTable::Query(int lang, const CU_STR& tid) const
{
	if (lang < 0) {
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

void TextTable::Insert(const CU_STR& tid, const std::vector<CU_STR>& languages)
{
	m_texts.insert(std::make_pair(tid, languages));
}

void TextTable::Clear()
{
	m_texts.clear();
}

#endif // S2_TEXT_TABLE_DB

}