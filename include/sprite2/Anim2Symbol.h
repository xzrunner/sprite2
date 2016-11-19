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
	virtual ~Anim2Symbol();
		
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	void SetAnim(rg_animation* anim) { m_anim = anim; }
	const rg_animation* GetAnim() const { return m_anim; }

protected:
	rg_animation* m_anim;

}; // Anim2Symbol

}

#endif // _SPRITE2_ANIM2_SYMBOL_H_