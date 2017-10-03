#ifndef _SPRITE2_SKELETON_SYMBOL_H_
#define _SPRITE2_SKELETON_SYMBOL_H_

#include "S2_Symbol.h"
#include "Skeleton.h"

#include <vector>

namespace s2
{

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
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const { return RENDER_SKIP; }

	void SetSkeleton(std::unique_ptr<Skeleton>& skeleton) { m_skeleton = std::move(m_skeleton); }
	const std::unique_ptr<Skeleton>& GetSkeleton() const { return m_skeleton; }

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

protected:
	std::unique_ptr<Skeleton> m_skeleton;

}; // SkeletonSymbol

}

#endif // _SPRITE2_SKELETON_SYMBOL_H_