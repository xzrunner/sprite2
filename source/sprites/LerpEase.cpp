#include "LerpEase.h"

#include <SM_Ease.h>

namespace s2
{

LerpEase::LerpEase()
	: m_type(EASE_UNKNOWN)
{
}

LerpEase::LerpEase(const LerpEase& lerp)
	: m_type(lerp.m_type)
{
}

LerpEase& LerpEase::operator = (const LerpEase& lerp)
{
	m_type = lerp.m_type;
	return *this;
}

LerpEase::LerpEase(int type)
	: m_type(type)
{
}

float LerpEase::GetRealTime(float t) const
{
	float ret = t;
	switch (m_type)
	{
	case EASE_LINEAR:
		ret = sm::linear(t);
		break;

	case EASE_IN_QUAD:
		ret = sm::in_quad(t);
		break;
	case EASE_IN_CUBIC:
		ret = sm::in_cubic(t);
		break;
	case EASE_IN_QUART:
		ret = sm::in_quart(t);
		break;
	case EASE_IN_QUINT:
		ret = sm::in_quint(t);
		break;
	case EASE_IN_SINE:
		ret = sm::in_sine(t);
		break;
	case EASE_IN_EXPO:
		ret = sm::in_expo(t);
		break;
	case EASE_IN_CIRC:
		ret = sm::in_circ(t);
		break;
	case EASE_IN_ELASTIC:
		ret = sm::in_elastic(t);
		break;
	case EASE_IN_BACK:
		ret = sm::in_back(t);
		break;
	case EASE_IN_BOUNCE:
		ret = sm::in_bounce(t);
		break;

	case EASE_OUT_QUAD:
		ret = sm::out_quad(t);
		break;
	case EASE_OUT_CUBIC:
		ret = sm::out_cubic(t);
		break;
	case EASE_OUT_QUART:
		ret = sm::out_quart(t);
		break;
	case EASE_OUT_QUINT:
		ret = sm::out_quint(t);
		break;
	case EASE_OUT_SINE:
		ret = sm::out_sine(t);
		break;
	case EASE_OUT_EXPO:
		ret = sm::out_expo(t);
		break;
	case EASE_OUT_CIRC:
		ret = sm::out_circ(t);
		break;
	case EASE_OUT_ELASTIC:
		ret = sm::out_elastic(t);
		break;
	case EASE_OUT_BACK:
		ret = sm::out_back(t);
		break;
	case EASE_OUT_BOUNCE:
		ret = sm::out_bounce(t);
		break;

	case EASE_IN_OUT_QUAD:
		ret = sm::in_out_quad(t);
		break;
	case EASE_IN_OUT_CUBIC:
		ret = sm::in_out_cubic(t);
		break;
	case EASE_IN_OUT_QUART:
		ret = sm::in_out_quart(t);
		break;
	case EASE_IN_OUT_QUINT:
		ret = sm::in_out_quint(t);
		break;
	case EASE_IN_OUT_SINE:
		ret = sm::in_out_sine(t);
		break;
	case EASE_IN_OUT_EXPO:
		ret = sm::in_out_expo(t);
		break;
	case EASE_IN_OUT_CIRC:
		ret = sm::in_out_circ(t);
		break;
	case EASE_IN_OUT_ELASTIC:
		ret = sm::in_out_elastic(t);
		break;
	case EASE_IN_OUT_BACK:
		ret = sm::in_out_back(t);
		break;
	case EASE_IN_OUT_BOUNCE:
		ret = sm::in_out_bounce(t);
		break;
	}
	return ret;
}

}