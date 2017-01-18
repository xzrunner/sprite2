#ifndef _SPRITE2_POINT_QUERY2_VISITOR_H_
#define _SPRITE2_POINT_QUERY2_VISITOR_H_

#include "SprVisitor.h"
#include "SprTreePath.h"
#include "SprVisitorParams.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Actor;

class PointQuery2Visitor : public SprVisitor
{
public:
	PointQuery2Visitor(const sm::vec2& pos);
	virtual ~PointQuery2Visitor();

	virtual VisitResult Visit(const Sprite* spr, const SprVisitorParams& params);

	virtual void VisitChildrenBegin(const Sprite* spr, const SprVisitorParams& params);
	virtual void VisitChildrenEnd(const Sprite* spr, const SprVisitorParams& params);

	Actor* GetSelectedActor() const;
	const sm::mat4& GetSelectedMat() const { return m_selected_params.mt; }
	
private:
	sm::vec2 m_pos;

	const Sprite*    m_selected_spr;
	SprVisitorParams m_selected_params;

	bool m_layer_find;

}; // PointQuery2Visitor

}

#endif // _SPRITE2_POINT_QUERY2_VISITOR_H_