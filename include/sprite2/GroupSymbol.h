#ifndef _SPRITE2_GROUP_SYMBOL_H_
#define _SPRITE2_GROUP_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class Sprite;

class GroupSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	GroupSymbol(const std::vector<Sprite*>& group);
	virtual ~GroupSymbol();
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const {}

	const std::vector<Sprite*>& GetAllChildren() const { return m_group; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	std::vector<Sprite*> m_group;

}; // GroupSymbol

}

#endif // _SPRITE2_GROUP_SYMBOL_H_