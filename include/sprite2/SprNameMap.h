#ifndef _SPRITE2_SPR_NAME_MAP_H_
#define _SPRITE2_SPR_NAME_MAP_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

// -1: not exists
// 0 -- 1023: static id
// 1024 -- 65535: start with '_'
// 65536 -- ..: others

class SprNameMap
{
public:
	static const int BASE_ID = 0;
	static const int MASK_ID = 1;

public:
	int  StrToID(const CU_STR& str);
	bool IDToStr(int id, CU_STR& str);

	int QueryID(const CU_STR& str);

	static bool IsTmpName(int id);
	static bool IsNormalName(int id);

	static bool IsEmptyName(int id);

private:
	enum NameType
	{
		NAME_STATIC = 0,
		NAME_TMP,
		NAME_NORMAL
	};

	int Insert(const CU_STR& str);
	int Insert(const CU_STR& str, NameType type);
	void Insert(const CU_STR& str, int id);

private:
	static const int MAX_STATIC_ID = 1024;
	static const int MAX_TMP_ID    = 65536;

private:
	CU_MAP<CU_STR, int> m_str2id;
	CU_MAP<int, CU_STR> m_id2str;

	int m_next_static_id, m_next_tmp_id, m_next_normal_id;

	CU_SINGLETON_DECLARATION(SprNameMap);

}; // SprNameMap

}

#endif // _SPRITE2_SPR_NAME_MAP_H_