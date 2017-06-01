#ifndef _SPRITE2_SPR_NAME_MAP_H_
#define _SPRITE2_SPR_NAME_MAP_H_

#include <CU_Singleton.h>

#include <string>
#include <map>

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
	int  StrToID(const std::string& str);
	bool IDToStr(int id, std::string& str);

	int QueryID(const std::string& str);

	static bool IsTmpName(int id);
	static bool IsNormalName(int id);

private:
	enum NameType
	{
		NAME_STATIC = 0,
		NAME_TMP,
		NAME_NORMAL
	};

	int Insert(const std::string& str);
	int Insert(const std::string& str, NameType type);
	void Insert(const std::string& str, int id);

private:
	static const int MAX_STATIC_ID = 1024;
	static const int MAX_TMP_ID    = 65536;

private:
	std::map<std::string, int> m_str2id;
	std::map<int, std::string> m_id2str;

	int m_next_static_id, m_next_tmp_id, m_next_normal_id;

	SINGLETON_DECLARATION(SprNameMap);

}; // SprNameMap

}

#endif // _SPRITE2_SPR_NAME_MAP_H_