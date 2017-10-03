#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "S2_Symbol.h"
#include "Scale9.h"

namespace s2
{

class Scale9Symbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Scale9Symbol();
	Scale9Symbol(uint32_t id);
	virtual ~Scale9Symbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor);
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;

	const Scale9& GetScale9() const { return m_s9; }
	Scale9& GetScale9() { return m_s9; }

	void Resize(float width, float height);

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

protected:
	Scale9 m_s9;

}; // Scale9Symbol

}

#endif // _SPRITE2_SCALE9_SYMBOL_H_	