#ifndef _SPRITE2_FLATTEN_MGR_H_
#define _SPRITE2_FLATTEN_MGR_H_

#include <CU_Singleton.h>

#include <map>

namespace s2
{

class SymbolFlatten;

class FlattenMgr
{
public:
	void Add(int sym_id, SymbolFlatten* flatten);
	void Delete(int sym_id);

	void UpdateTexcoords();

private:
	std::map<int, SymbolFlatten*> m_id2flatten;

	SINGLETON_DECLARATION(FlattenMgr);

}; // FlattenMgr

}

#endif // _SPRITE2_FLATTEN_MGR_H_