#include "LerpSpiral.h"

#include <SM_Calc.h>

namespace s2
{

LerpSpiral::LerpSpiral()
	: m_angle_begin(0)
	, m_angle_end(0)
	, m_scale(1)
{
}

LerpSpiral::LerpSpiral(const LerpSpiral& lerp)
	: m_angle_begin(lerp.m_angle_begin)
	, m_angle_end(lerp.m_angle_end)
	, m_scale(lerp.m_scale)
{
}

LerpSpiral& LerpSpiral::operator = (const LerpSpiral& lerp)
{
	m_angle_begin = lerp.m_angle_begin;
	m_angle_end = lerp.m_angle_end;
	m_scale = lerp.m_scale;
	return *this;
}

LerpSpiral::LerpSpiral(float angle_begin, float angle_end, float scale)
	: m_angle_begin(angle_begin)
	, m_angle_end(angle_end)
	, m_scale(scale)
{
}

sm::vec2 LerpSpiral::Lerp(const sm::vec2& begin, const sm::vec2& end, float process) const
{
	float angle = m_angle_begin + (m_angle_end - m_angle_begin) * process;

	float dis = sm::dis_pos_to_pos(begin, end);
	float r = dis * process;

	sm::vec2 ret;

	ret.x = r * cos(angle);
	ret.y = r * sin(angle);

	if (m_scale != 1) 
	{
		float dir = sm::get_line_angle(begin, end);
		ret = sm::rotate_vector(ret, -dir);
		ret.y *= m_scale;
		ret = sm::rotate_vector(ret, dir);
	}

	ret += begin;

	return ret;
}

void LerpSpiral::GetAngle(float& begin, float& end) const
{
	begin = m_angle_begin;
	end = m_angle_end;
}

}