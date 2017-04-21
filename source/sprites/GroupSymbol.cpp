#include "GroupSymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"

namespace s2
{

GroupSymbol::GroupSymbol(const std::vector<Sprite*>& group)
	: m_group(group)
{
	for_each(m_group.begin(), m_group.end(), cu::AddRefFunctor<Sprite>());
}

GroupSymbol::~GroupSymbol()
{
	for_each(m_group.begin(), m_group.end(), cu::RemoveRefFunctor<Sprite>());
}

int GroupSymbol::Type() const
{
	return SYM_GROUP;
}

sm::rect GroupSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect();
}

}