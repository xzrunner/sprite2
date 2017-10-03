#include "SpriteVisitor.h"
#include "S2_Sprite.h"

#include <assert.h>

namespace s2
{

SpriteVisitor::SpriteVisitor(bool order)
	: m_order(order)
{
}

bool SpriteVisitor::VisitChild(SpriteVisitor& visitor, const SprVisitorParams& params, const SprConstPtr& child, VisitResult& ret)
{
	bool stop = false;
	VisitResult v = child->Traverse(visitor, params);
	switch (v)
	{
	case VISIT_INTO:
		assert(0);
		break;
	case VISIT_OUT:
		stop = true;
		break;
	case VISIT_STOP:
		ret = VISIT_STOP;
		stop = true;
		break;
	}
	return !stop;
}

}