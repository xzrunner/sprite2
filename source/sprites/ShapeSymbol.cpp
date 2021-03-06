#include "sprite2/ShapeSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

ShapeSymbol::ShapeSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

ShapeSymbol::ShapeSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

ShapeSymbol::~ShapeSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS
}

int ShapeSymbol::Type() const 
{ 
	return SYM_SHAPE; 
}

pt2::RenderReturn ShapeSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SHAPE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS

	if (!m_shape || !spr) {
		return pt2::RENDER_NO_DATA;
	}

	pt2::RenderReturn ret = pt2::RENDER_OK;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (DrawNode::Prepare(rp, spr, *rp_child)) {
		// todo: shape's render ret
		m_shape->Draw(dlist, *rp_child);
		ret = pt2::RENDER_OK;
	} else {
		ret = pt2::RENDER_INVISIBLE;
	}

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn ShapeSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!m_shape || !spr) {
		return pt2::RENDER_NO_DATA;
	}

	m_shape->Draw(dlist, rp);

	return pt2::RENDER_OK;
}
#endif // S2_DISABLE_FLATTEN

sm::rect ShapeSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_shape) {
		return m_shape->GetBounding();
	} else {
		return sm::rect(0, 0);
	}
}

}