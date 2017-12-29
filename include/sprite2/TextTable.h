#ifndef _SPRITE2_TEXT_TABLE_H_
#define _SPRITE2_TEXT_TABLE_H_

#include "sprite2/config.h"

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>
#ifdef S2_TEXT_TABLE_DB
#include <playdb/btree/BTree.h>
#endif // S2_TEXT_TABLE_DB

namespace s2
{

class TextTable
{
public:	
	void SetTypes(const CU_VEC<CU_STR>& types) {
		m_types = types;
	}

	CU_STR Query(int lang, const CU_STR& tid) const;

	int QueryType(const CU_STR& lang) const;

#ifdef S2_TEXT_TABLE_DB
	void LoadDB(const CU_STR& index_path, const CU_STR& data_path);
#else
	void Insert(const CU_STR& tid, const CU_VEC<CU_STR>& languages);

	void Clear();

	bool IsEmpty() const { return m_texts.empty(); }
#endif // S2_TEXT_TABLE_DB

private:
	CU_VEC<CU_STR> m_types;

#ifdef S2_TEXT_TABLE_DB
	std::unique_ptr<playdb::IStorageManager> m_storage_mgr;
	std::unique_ptr<playdb::btree::BTree<CU_STR>> m_tree;
#else
	CU_MAP<CU_STR, CU_VEC<CU_STR>> m_texts;
#endif // S2_TEXT_TABLE_DB

	CU_SINGLETON_DECLARATION(TextTable)

}; // TextTable

}

#endif // _SPRITE2_TEXT_TABLE_H_