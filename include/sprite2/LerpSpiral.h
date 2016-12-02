#ifndef _SPRITE2_LERP_SPIRAL_H_
#define _SPRITE2_LERP_SPIRAL_H_

#include "ILerp.h"
#include "LerpType.h"

#include <SM_Vector.h>

namespace s2
{

class LerpSpiral : public ILerp
{
public:
	LerpSpiral();
	LerpSpiral(const LerpSpiral& lerp);
	LerpSpiral& operator = (const LerpSpiral& lerp);
	LerpSpiral(float angle_begin, float angle_end, float scale = 1);

	virtual ILerp* Clone() const { return new LerpSpiral(*this); }

	virtual int Type() const { return LERP_SPIRAL; }

	sm::vec2 Lerp(const sm::vec2& begin, const sm::vec2& end, float process) const;

	void GetAngle(float& begin, float& end) const;
	float GetScale() const { return m_scale; }
	
private:
	float m_angle_begin, m_angle_end;
	float m_scale;
	
}; // LerpSpiral

}

#endif // _SPRITE2_LERP_SPIRAL_H_