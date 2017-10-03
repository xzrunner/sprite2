#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#include "SprTreePath.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>

namespace s2
{

class Actor;

class PointQueryVisitor : public SpriteVisitor
{
public:
	PointQueryVisitor(const sm::vec2& pos);

	virtual VisitResult Visit(const SprConstPtr& spr, const SprVisitorParams& params);

	virtual VisitResult VisitChildrenBegin(const SprConstPtr& spr, const SprVisitorParams& params);
	virtual VisitResult VisitChildrenEnd(const SprConstPtr& spr, const SprVisitorParams& params);

	ActorConstPtr GetSelectedActor() const;
	const S2_MAT& GetSelectedMat() const { return m_selected_params.mt; }
	
private:
	bool QuerySprite(const SprConstPtr& spr, const SprVisitorParams& params) const;
	bool IsPointInScissor(const SprConstPtr& spr, const SprVisitorParams& params) const;

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
		std::vector<bool> m_editable;
		std::vector<bool> m_visible;

	}; // SprPath

private:
	sm::vec2 m_pos;

	SprConstPtr m_selected_spr;
	SprVisitorParams m_selected_params;

	SprPath m_curr_path;
	SprPath m_selected_path;

	bool m_finded;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_