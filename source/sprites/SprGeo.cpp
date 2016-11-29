#include "SprGeo.h"

namespace s2
{

sm::vec2 SprGeo::DEFAULT_POSITION(0, 0);
float    SprGeo::DEFAULT_ANGLE(0);
sm::vec2 SprGeo::DEFAULT_SCALE(1, 1);
sm::vec2 SprGeo::DEFAULT_SHEAR(0, 0);

SprGeo   SprGeo::DEFAULT_GEO;

SprGeo::SprGeo()
	: m_position(DEFAULT_POSITION)
	, m_angle(DEFAULT_ANGLE)
	, m_scale(DEFAULT_SCALE)
	, m_shear(DEFAULT_SHEAR)
{
	m_offset.MakeInvalid();
}

}