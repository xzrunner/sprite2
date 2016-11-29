#include "SprGeo.h"

namespace s2
{

SprGeo::SprGeo()
	: m_position(0, 0)
	, m_angle(0)
	, m_scale(1, 1)
	, m_shear(0, 0)
{
	m_offset.MakeInvalid();
}

}