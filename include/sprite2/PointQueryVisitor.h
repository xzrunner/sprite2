#ifndef _SPRITE2_POINT_QUERY_VISITOR_H_
#define _SPRITE2_POINT_QUERY_VISITOR_H_

#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#include "SprTreePath.h"

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

	const Actor* GetSelectedActor() const;
	const S2_MAT& GetSelectedMat() const { return m_selected_params.mt; }
	
private:
	bool QuerySprite(const Sprite* spr, const SprVisitorParams& params) const;

private:
	class SprPath
	{
	public:

		bool IsPartOf(const SprPath& long_path) const;

		bool Empty() const;

		void Push(int spr_id, bool editable);
		void Pop();

		bool IsEditable() const;

	private:
		SprTreePath m_impl;
		std::vector<bool> m_editable;

	}; // SprPath

private:
	sm::vec2 m_pos;

	const Sprite*    m_selected_spr;
	SprVisitorParams m_selected_params;

	SprPath m_curr_path;
	SprPath m_selected_path;

	bool m_finded;

}; // PointQueryVisitor

}

#endif // _SPRITE2_POINT_QUERY_VISITOR_H_