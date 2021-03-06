#ifndef _SPRITE2_BEZIER_SHAPE_H_
#define _SPRITE2_BEZIER_SHAPE_H_

#include "sprite2/PolylineShape.h"

namespace s2
{

class BezierShape : public PolylineShape
{
public:
	BezierShape();
	BezierShape(const BezierShape& bezier);
	BezierShape& operator = (const BezierShape& bezier);
	BezierShape(const sm::vec2& start, const sm::vec2& end);
	
	virtual BezierShape* Clone() const override;
	virtual int Type() const override { return SHAPE_BEZIER; }
	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;

// 	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }
// 	void SetCtrlNodes(const sm::vec2* ctrl_nodes) {
	// 	memcpy(m_control_nodes, ctrl_nodes, sizeof(m_control_nodes));
	// 	UpdateCurve();
//}

protected:
	virtual void UpdatePolyline();

private:
	sm::vec2 PointOnCubicBezier(float t);

public:
	static const int CTRL_NODE_COUNT = 4;

protected:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	sm::vec2 m_control_nodes[CTRL_NODE_COUNT];

}; // BezierShape

}

#endif // _SPRITE2_BEZIER_SHAPE_H_