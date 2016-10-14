#ifndef _SPRITE2_ANIM2_SYMBOL_H_
#define _SPRITE2_ANIM2_SYMBOL_H_

#include "S2_Symbol.h"

#include <rigging/rg_skeleton.h>

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

	void SetSkeleton(rg_skeleton* skeleton) { m_skeleton = skeleton; }
	const rg_skeleton* GetSkeleton() const { return m_skeleton; }

protected:
	rg_skeleton* m_skeleton;

}; // Anim2Symbol

}

#endif // _SPRITE2_ANIM2_SYMBOL_H_