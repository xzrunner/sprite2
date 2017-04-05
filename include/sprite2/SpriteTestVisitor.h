#ifndef _SPRITE2_SPRITE_TEST_VISITOR_H_
#define _SPRITE2_SPRITE_TEST_VISITOR_H_

#include "SpriteVisitor.h"
#include "SprVisitorParams.h"

#include <SM_Vector.h>

namespace s2
{

class SpriteTestVisitor : public SpriteVisitor
{
public:
	SpriteTestVisitor(const sm::vec2& pos);
	virtual ~SpriteTestVisitor();

	virtual VisitResult Visit(const Sprite* spr, SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const Sprite* spr, SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const Sprite* spr, SprVisitorParams& params);

	const Actor* GetSelectedActor() const;
	
private:
	bool QueryScissor(const Sprite* spr, const SprVisitorParams& params, bool* out_has_scissor) const;
	bool QuerySprite(const Sprite* spr, const SprVisitorParams& params) const;

private:
	sm::vec2 m_pos;
	SprVisitorParams m_selected_params;
}; // SpriteTestVisitor

}

#endif // _SPRITE2_SPRITE_TEST_VISITOR_H_