#ifndef _SPRITE2_DYNAMIC_RECT_ICON_H_
#define _SPRITE2_DYNAMIC_RECT_ICON_H_

#include "sprite2/Icon.h"

namespace s2
{

class DynamicRectIcon : public VIRTUAL_INHERITANCE Icon
{
public:
	DynamicRectIcon();

	virtual DynamicRectIcon* Clone() const override { return new DynamicRectIcon(*this); }

	void SetRegion(const sm::rect& begin, const sm::rect& end);
	void GetRegion(sm::rect& begin, sm::rect& end) const;
	
protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const override;

protected:
	sm::rect m_begin, m_end;

}; // DynamicRectIcon

}

#endif // _SPRITE2_DYNAMIC_RECT_ICON_H_