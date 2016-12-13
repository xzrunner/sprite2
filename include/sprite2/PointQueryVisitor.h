#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SprVisitor.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class PointQueryVisitor : public SprVisitor
{
public:
	PointQueryVisitor(const sm::vec2& pos);
	virtual ~PointQueryVisitor();

	virtual VisitResult Visit(const Sprite* spr, const sm::mat4* mat);

	virtual void VisitChildrenBegin(const Sprite* spr);
	virtual void VisitChildrenEnd(const Sprite* spr);

	const Sprite* GetSelectedSpr() const { return m_spr; }
	const sm::mat4& GetSelectedMat() const { return m_mat; }
	
private:
	sm::vec2 m_pos;

	const Sprite* m_spr;
	sm::mat4 m_mat;

	bool m_layer_find;
	std::vector<const Sprite*> m_parents;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_