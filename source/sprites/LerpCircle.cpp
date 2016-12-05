#include "LerpCircle.h"

#include <SM_Calc.h>

namespace s2
{

LerpCircle::LerpCircle()
	: m_scale(1)
{
}

LerpCircle::LerpCircle(const LerpCircle& lerp)
	: m_scale(lerp.m_scale)
{
}

LerpCircle& LerpCircle::operator = (const LerpCircle& lerp)
{
	m_scale = lerp.m_scale;
	return *this;
}

LerpCircle::LerpCircle(float scale)
	: m_scale(scale)
{
}

sm::vec2 LerpCircle::Lerp(const sm::vec2& begin, const sm::vec2& end, float process) const
{
	sm::vec2 c = (begin + end) * 0.5f;
	float angle = SM_PI * process;

	sm::vec2 ret = sm::rotate_vector(begin - c, angle);

	if (m_scale != 1) 
	{
		float dir = sm::get_line_angle(begin, end);
		ret = sm::rotate_vector(ret, -dir);
		ret.y *= m_scale;
		ret = sm::rotate_vector(ret, dir);
	}

	return ret;
}

}