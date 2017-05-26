#ifndef _SPRITE2_ANIM2_SYMBOL_H_
#define _SPRITE2_ANIM2_SYMBOL_H_

#include "S2_Symbol.h"

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
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;

	void SetAnim(rg_animation* anim);
	const rg_animation* GetAnim() const { return m_anim; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	rg_animation* m_anim;

}; // Anim2Symbol

}

#endif // _SPRITE2_ANIM2_SYMBOL_H_