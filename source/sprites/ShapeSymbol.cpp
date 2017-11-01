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

RenderReturn ShapeSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SHAPE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SHAPE);
#endif // S2_DISABLE_STATISTICS

	if (!m_shape || !spr) {
		return RENDER_NO_DATA;
	}

	RenderReturn ret = RENDER_OK;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (DrawNode::Prepare(rp, spr, *rp_child)) {
		// todo: shape's render ret
		m_shape->Draw(*rp_child);
		ret = RENDER_OK;
	} else {
		ret = RENDER_INVISIBLE;
	}

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
RenderReturn ShapeSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!m_shape || !spr) {
		return RENDER_NO_DATA;
	}

	m_shape->Draw(rp);

	return RENDER_OK;
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