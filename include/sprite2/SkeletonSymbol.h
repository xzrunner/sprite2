#ifndef _SPRITE2_SKELETON_SYMBOL_H_
#define _SPRITE2_SKELETON_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class Skeleton;

class SkeletonSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	SkeletonSymbol();
	SkeletonSymbol(uint32_t id);
	virtual ~SkeletonSymbol();
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const;

	void SetSkeleton(Skeleton* skeleton);
	const Skeleton* GetSkeleton() const { return m_skeleton; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	Skeleton* m_skeleton;

}; // SkeletonSymbol

}

#endif // _SPRITE2_SKELETON_SYMBOL_H_