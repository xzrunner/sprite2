#ifndef _SPRITE2_STATIC_QUAD_ICON_H_
#define _SPRITE2_STATIC_QUAD_ICON_H_

#include "sprite2/Icon.h"

namespace s2
{

class StaticQuadIcon : public VIRTUAL_INHERITANCE Icon
{
public:
	StaticQuadIcon();
	StaticQuadIcon(const StaticQuadIcon& icon);
	StaticQuadIcon& operator = (const StaticQuadIcon& icon);

	virtual StaticQuadIcon* Clone() const override { return new StaticQuadIcon(*this); }

	virtual RenderReturn Draw(cooking::DisplayList* dlist, const RenderParams& rp, float process) const override;

	virtual sm::rect GetRegion(float process) const override;

	const sm::vec2* GetScreen() const { return m_screen; }
	void SetScreen(const sm::vec2* screen);

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const override;

	virtual void Update() override;

protected:
	sm::vec2 m_src[4], m_screen[4];

}; // StaticQuadIcon

}

#endif // _SPRITE2_STATIC_QUAD_ICON_H_