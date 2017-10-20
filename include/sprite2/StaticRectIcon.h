#ifndef _SPRITE2_STATIC_RECT_ICON_H_
#define _SPRITE2_STATIC_RECT_ICON_H_

#include "Icon.h"

namespace s2
{

class StaticRectIcon : public VIRTUAL_INHERITANCE Icon
{
public:
	StaticRectIcon();
	
	virtual StaticRectIcon* Clone() const { return new StaticRectIcon(*this); }

	void SetRegion(const sm::rect& r);
	void GetRegion(sm::vec2& min, sm::vec2& max) const;

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const;

protected:
	sm::vec2 m_min, m_max;

}; // StaticRectIcon

}

#endif // _SPRITE2_STATIC_RECT_ICON_H_