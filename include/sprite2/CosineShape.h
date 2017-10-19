#ifndef _SPRITE2_COSINE_SHAPE_H_
#define _SPRITE2_COSINE_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class CosineShape : public PolylineShape
{
public:
	CosineShape();
	CosineShape(const CosineShape& cosine);
	CosineShape(const CU_VEC<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual CosineShape* Clone() const;	

	/**
	 *  @interface
	 *    Shape
	 */
	virtual int Type() const { return SHAPE_COSINE; }
	virtual void Draw(const RenderParams& rp) const;

private:
	void UpdatePolyline();

protected:
	CU_VEC<sm::vec2> m_mid_points;

}; // CosineShape

}

#endif // _SPRITE2_COSINE_SHAPE_H_