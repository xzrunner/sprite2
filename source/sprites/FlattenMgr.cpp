#include "FlattenMgr.h"
#include "SymbolFlatten.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(FlattenMgr);

FlattenMgr::FlattenMgr()
{
}

void FlattenMgr::Add(int sym_id, SymbolFlatten* flatten)
{
	std::pair<std::map<int, SymbolFlatten*>::iterator, bool> ret 
		= m_id2flatten.insert(std::make_pair(sym_id, flatten));
	assert(ret.second);
}

void FlattenMgr::Delete(int sym_id)
{
	std::map<int, SymbolFlatten*>::iterator itr = m_id2flatten.find(sym_id);
	if (itr != m_id2flatten.end()) {
		m_id2flatten.erase(itr);
	}
}

void FlattenMgr::UpdateTexcoords()
{
	std::map<int, SymbolFlatten*>::iterator itr 
		= m_id2flatten.begin();
	for ( ; itr != m_id2flatten.end(); ++itr) {
		itr->second->UpdateTexcoords();
	}
}

}