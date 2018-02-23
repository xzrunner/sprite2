#ifndef _SPRITE2_SKELETON_SYMBOL_H_
#define _SPRITE2_SKELETON_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/Skeleton.h"

#include <cu/cu_stl.h>

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return pt2::RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetSkeleton(std::unique_ptr<Skeleton>& skeleton) { m_skeleton = std::move(m_skeleton); }
	const std::unique_ptr<Skeleton>& GetSkeleton() const { return m_skeleton; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

protected:
	std::unique_ptr<Skeleton> m_skeleton = nullptr;

}; // SkeletonSymbol

}

#endif // _SPRITE2_SKELETON_SYMBOL_H_