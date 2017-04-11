#ifndef _SPRITE2_SET_STATIC_FRAME_VISITOR_H_
#define _SPRITE2_SET_STATIC_FRAME_VISITOR_H_

#include "SpriteVisitor.h"

namespace s2
{

class SetStaticFrameVisitor : public SpriteVisitor
{
public:
	SetStaticFrameVisitor(int static_frame);
	
	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

private:
	int m_static_frame;

}; // SetStaticFrameVisitor

}

#endif // _SPRITE2_SET_STATIC_FRAME_VISITOR_H_