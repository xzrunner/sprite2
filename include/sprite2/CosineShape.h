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
	CosineShape(const std::vector<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual CosineShape* Clone() const;	

	/**
	 *  @interface
	 *    Shape
	 */
	virtual void Draw(const S2_MAT& mt, const RenderColor* color = NULL) const;

private:
	void UpdatePolyline();

protected:
	std::vector<sm::vec2> m_mid_points;

}; // CosineShape

}

#endif // _SPRITE2_COSINE_SHAPE_H_