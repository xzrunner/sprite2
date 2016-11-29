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
	static sm::vec2 DEFAULT_POSITION;
	static float    DEFAULT_ANGLE;
	static sm::vec2 DEFAULT_SCALE;
	static sm::vec2 DEFAULT_SHEAR;

	static SprGeo	DEFAULT_GEO;
	
private:
	sm::vec2 m_position;
	float    m_angle;
	sm::vec2 m_scale;
	sm::vec2 m_shear;

	sm::vec2 m_offset;

	friend class Sprite;

}; // SprGeo

}

#endif // _SPRITE2_SPR_GEO_H_
