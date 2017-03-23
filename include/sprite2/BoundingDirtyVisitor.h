#ifndef _SPRITE2_BOUNDING_DIRTY_VISITOR_H_
#define _SPRITE2_BOUNDING_DIRTY_VISITOR_H_

#include "SpriteVisitor.h"

#include <SM_Rect.h>

namespace s2
{

class BoundingDirtyVisitor : public SpriteVisitor
{
public:
	BoundingDirtyVisitor();

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	bool IsDirty() const { return m_bounding_dirty; }

private:
	bool m_bounding_dirty;

}; // BoundingDirtyVisitor

}

#endif // _SPRITE2_BOUNDING_DIRTY_VISITOR_H_