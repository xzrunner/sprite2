#include "SprVisitor.h"
#include "S2_Sprite.h"

#include <assert.h>

namespace s2
{

SprVisitor::SprVisitor(bool order)
	: m_order(order)
{
}

bool SprVisitor::VisitChild(SprVisitor& visitor, const SprVisitorParams& params, Sprite* child, VisitResult& ret)
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