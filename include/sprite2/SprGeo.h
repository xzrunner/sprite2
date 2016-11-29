#ifndef _SPRITE2_SPR_GEO_H_
#define _SPRITE2_SPR_GEO_H_

#include <SM_Vector.h>

namespace s2
{

class SprGeo
{
public:
	SprGeo();

private:
	sm::vec2 m_position;
	float    m_angle;
	sm::vec2 m_scale;
	sm::vec2 m_shear;

	sm::vec2 m_offset;

	friend class Sprite;
	friend class SprDefault;

}; // SprGeo

}

#endif // _SPRITE2_SPR_GEO_H_
