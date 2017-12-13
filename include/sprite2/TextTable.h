#ifndef _SPRITE2_TEXT_TABLE_H_
#define _SPRITE2_TEXT_TABLE_H_

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>

namespace s2
{

class TextTable
{
public:
	enum LanguageIdx
	{
		LANG_SIMPLIFIED = 0,
		LANG_TRADITIONAL = 1,

		LANG_COUNT
	};

public:
	void Insert(const CU_STR& tid, const CU_VEC<CU_STR>& languages);
	
	CU_STR Query(LanguageIdx lang, const CU_STR& tid) const;

	void Clear();

	bool IsEmpty() const { return m_texts.empty(); }

private:
	CU_MAP<CU_STR, CU_VEC<CU_STR>> m_texts;

	CU_SINGLETON_DECLARATION(TextTable)

}; // TextTable

}

#endif // _SPRITE2_TEXT_TABLE_H_