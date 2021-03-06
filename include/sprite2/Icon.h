#ifndef _SPRITE2_ICON_H_
#define _SPRITE2_ICON_H_

#include "sprite2/pre_defined.h"

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <painting2/RenderReturn.h>

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class RenderParams;
class ImageSymbol;

class Icon
{
public:
	virtual ~Icon() {}

	virtual Icon* Clone() const = 0;

	virtual pt2::RenderReturn Draw(cooking::DisplayList* dlist, const RenderParams& rp, float process) const;
	
	virtual sm::rect GetRegion(float process) const;

	void GenTexcoords(float process, sm::vec2* texcoords) const;
	void GenVertices(float process, const sm::vec2* texcoords,
		sm::vec2* vertices) const;

	void SetImage(const std::shared_ptr<ImageSymbol>& img);
	const std::shared_ptr<ImageSymbol>& GetImage() const { return m_img; }

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const = 0;
	
	virtual void Update() {}

protected:
	std::shared_ptr<ImageSymbol> m_img = nullptr;

}; // Icon

}

#endif // _SPRITE2_ICON_H_