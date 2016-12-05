#ifndef _SPRITE2_LERP_CIRCLE_H_
#define _SPRITE2_LERP_CIRCLE_H_

#include "ILerp.h"
#include "LerpType.h"

#include <SM_Vector.h>

namespace s2
{

class LerpCircle : public ILerp
{
public:
	LerpCircle();
	LerpCircle(const LerpCircle& lerp);
	LerpCircle& operator = (const LerpCircle& lerp);
	LerpCircle(float scale);

	virtual ILerp* Clone() const { return new LerpCircle(*this); }

	virtual int Type() const { return LERP_CIRCLE; }

	sm::vec2 Lerp(const sm::vec2& begin, const sm::vec2& end, float process) const;

	float GetScale() const { return m_scale; }
	
private:
	float m_scale;
	
}; // LerpCircle

}

#endif // _SPRITE2_LERP_CIRCLE_H_