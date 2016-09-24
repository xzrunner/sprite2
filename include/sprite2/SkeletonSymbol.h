#ifndef _SPRITE2_SKELETON_SYMBOL_H_
#define _SPRITE2_SKELETON_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class SkeletonSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	SkeletonSymbol();
	virtual ~SkeletonSymbol();
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	
private:


}; // SkeletonSymbol

}

#endif // _SPRITE2_SKELETON_SYMBOL_H_