#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "sprite2/SpriteVisitor.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/SprTreePath.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

namespace s2
{

class Actor;

class PointQueryVisitor : public SpriteVisitor2
{
public:
	PointQueryVisitor(const sm::vec2& pos);

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams2& params);

	virtual VisitResult VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams2& params);
	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams2& params);

	ActorConstPtr GetSelectedActor() const;
	const S2_MAT& GetSelectedMat() const { return m_selected_params.mt; }
	
private:
	bool QuerySprite(const SprConstPtr& spr, const SprVisitorParams2& params) const;
	bool IsPointInScissor(const SprConstPtr& spr, const SprVisitorParams2& params) const;

	bool IsPointInRect(const sm::rect& rect, const S2_MAT& mat) const;	

private:
	class SprPath
	{
	public:

		bool IsPartOf(const SprPath& long_path) const;
		bool IsBatterThan(const SprPath& path) const;

		bool Empty() const;

		void Push(int spr_id, bool editable, bool visible);
		void Pop();

		bool IsVisible() const;

	private:
		SprTreePath m_impl;
		CU_VEC<bool> m_editable;
		CU_VEC<bool> m_visible;

	}; // SprPath

private:
	sm::vec2 m_pos;

	SprConstPtr m_selected_spr;
	SprVisitorParams2 m_selected_params;

	SprPath m_curr_path;
	SprPath m_selected_path;

	bool m_finded;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_