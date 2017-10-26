#ifndef _SPRITE2_SET_STATIC_FRAME_VISITOR_H_
#define _SPRITE2_SET_STATIC_FRAME_VISITOR_H_

#include "sprite2/SpriteVisitor.h"

namespace s2
{

class SetStaticFrameVisitor : public SpriteVisitor
{
public:
	SetStaticFrameVisitor(int static_frame);
	
	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams& params);

private:
	int m_static_frame;

}; // SetStaticFrameVisitor

}

#endif // _SPRITE2_SET_STATIC_FRAME_VISITOR_H_