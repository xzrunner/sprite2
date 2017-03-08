#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SprVisitor.h"

#include <SM_Vector.h>

namespace s2
{

class PointQueryVisitor : public SprVisitor
{
public:
	PointQueryVisitor(const sm::vec2& pos);
	virtual ~PointQueryVisitor();

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params);

	const Sprite* GetSelectedSpr() const { return m_spr; }
	const S2_MAT& GetSelectedMat() const { return m_mat; }
	
private:
	sm::vec2 m_pos;

	const Sprite* m_spr;
	S2_MAT m_mat;

	bool m_layer_find;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_