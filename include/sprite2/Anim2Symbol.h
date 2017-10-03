#ifndef _SPRITE2_ANIM2_SYMBOL_H_
#define _SPRITE2_ANIM2_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

struct rg_animation;

namespace s2
{

class Anim2Symbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Anim2Symbol();
	Anim2Symbol(uint32_t id);
	virtual ~Anim2Symbol();
		
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;

	void SetAnim(rg_animation* anim);
	const rg_animation* GetAnim() const { return m_anim; }

	void AddCachedSym(const SymPtr& sym) { m_symbols.push_back(sym); }

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

protected:
	rg_animation* m_anim;

private:
	// cache anim used all symbol
	std::vector<SymPtr> m_symbols;

}; // Anim2Symbol

}

#endif // _SPRITE2_ANIM2_SYMBOL_H_