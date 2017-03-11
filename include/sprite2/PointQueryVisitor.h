#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SpriteVisitor.h"
#include "SprTreePath.h"
#include "SprVisitorParams.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Actor;

class PointQueryVisitor : public SpriteVisitor
{
public:
	PointQueryVisitor(const sm::vec2& pos);
	virtual ~PointQueryVisitor();

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params);

	Actor* GetSelectedActor() const;
	const S2_MAT& GetSelectedMat() const { return m_selected_params.mt; }
	
private:
	bool QuerySprite(const Sprite* spr, const sm::mat4& mat) const;

private:
	sm::vec2 m_pos;

	const Sprite*    m_selected_spr;
	SprVisitorParams m_selected_params;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_