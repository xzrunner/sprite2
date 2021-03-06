#ifndef _SPRITE2_LERP_WIGGLE_H_
#define _SPRITE2_LERP_WIGGLE_H_

#include "sprite2/ILerp.h"
#include "sprite2/LerpType.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

namespace s2
{

class LerpWiggle : public ILerp
{
public:
	LerpWiggle();
	LerpWiggle(const LerpWiggle& lerp);
	LerpWiggle& operator = (const LerpWiggle& lerp);
	LerpWiggle(float freq, float amp);

	virtual std::unique_ptr<ILerp> Clone() const override { return std::make_unique<LerpWiggle>(*this); }

	virtual int Type() const override { return LERP_WIGGLE; }

	sm::vec2 Lerp(const sm::vec2& v, float time) const;

	float GetFreq() const { return m_freq; }
	float GetAmp() const { return m_amp; }

private:
	float m_freq;	// frequency
	float m_amp;	// amplitude

	mutable CU_VEC<float> m_rands;

}; // LerpWiggle

}

#endif // _SPRITE2_LERP_WIGGLE_H_