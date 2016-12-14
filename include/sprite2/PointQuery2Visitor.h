#ifndef _SPRITE2_POINT_QUERY2_VISITOR_H_
#define _SPRITE2_POINT_QUERY2_VISITOR_H_

#include "SprVisitor.h"
#include "SprTreePath.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Actor;

class PointQuery2Visitor : public SprVisitor
{
public:
	PointQuery2Visitor(const SprTreePath& parent, const sm::vec2& pos);
	virtual ~PointQuery2Visitor();

	virtual VisitResult Visit(const Sprite* spr, const sm::mat4* mat);

	virtual void VisitChildrenBegin(const Sprite* spr);
	virtual void VisitChildrenEnd(const Sprite* spr);

	Actor* GetSelectedActor() const;
	const sm::mat4& GetSelectedMat() const { return m_mat; }
	
private:
	SprTreePath m_parent;
	sm::vec2 m_pos;

	const Sprite* m_spr;
	sm::mat4 m_mat;

	bool m_layer_find;

}; // PointQuery2Visitor

}

#endif // _SPRITE2_POINT_QUERY2_VISITOR_H_