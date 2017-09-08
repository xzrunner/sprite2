#ifndef _SPRITE2_DRAW_NODE_DEFER_H_
#define _SPRITE2_DRAW_NODE_DEFER_H_

#include "RenderParams.h"
#include "RenderReturn.h"

namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;

class DrawNodeDeferred
{
public:
	static RenderReturn Draw(cooking::DisplayList* dlist, const Sprite* spr, 
		const RenderParams& rp = RenderParams());

	/**
	 *  @return
	 *    bool, if is outside
	 */
	static bool CullingTestOutside(const Sprite* spr, const RenderParams& rp);

private:
	static RenderReturn DrawSprImpl(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp);

	static RenderReturn DrawSprImplFinal(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp);

}; // DrawNodeDeferred

}

#endif // _SPRITE2_DRAW_NODE_DEFER_H_