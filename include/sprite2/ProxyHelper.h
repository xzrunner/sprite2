#ifndef _SPRITE2_PROXY_HELPER_H_
#define _SPRITE2_PROXY_HELPER_H_

#include "s2_typedef.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>
#include <string>
#include <memory>

namespace s2
{

class Sprite;
class Actor;

class ProxyHelper
{
public:
	// should be release outside
	static SprPtr BuildGroup(const std::vector<std::pair<const ActorConstPtr, SprPtr>>& items, bool force_group = false);

	/************************************************************************/
	/* sprite                                                               */
	/************************************************************************/

	static bool SprGetPos(const SprPtr& spr, sm::vec2& pos);
	static bool SprGetAngle(const SprPtr& spr, float& angle);
	static bool SprGetScale(const SprPtr& spr, sm::vec2& scale);

	static bool SprGetSymID(const SprPtr& spr, int& id);
	static bool SprGetSymType(const SprPtr& spr, int& type);

	static bool SprPointTest(const SprPtr& spr, const sm::vec2& pos);
	static ActorConstPtr SprPointQuery(const SprPtr& spr, const sm::vec2& pos, float mat[6]);

	static bool SprGetForceUpdate(const SprPtr& spr, bool& force);
	static void SprSetForceUpdate(const SprPtr& spr, bool force);

	static void SprSetInheritUpdate(const SprPtr& spr, bool inherit);
	// ret: -1 unknown, 0 false, 1 true
	static int  SprGetInheritUpdate(const SprPtr& spr);

	// anim
	static bool SprGetFrameCount(const SprPtr& spr, int& count);
	static void SprAnimSetLoop(const SprPtr& spr, bool loop);

	// complex
	static bool SprHasAction(const SprPtr& spr, const std::string& action);

	// textbox
	static void SprTextboxResetTime(const SprPtr& spr);
	static void SprTextboxSetFontColor(const SprPtr& spr, uint32_t abgr);
	static void SprTextboxSetEdgeColor(const SprPtr& spr, uint32_t abgr);

	// anim2
	static void SprAnim2SetStaticTime(const SprPtr& spr, int time);

	// particle3d
	static void SprP3dSetLocal(const SprPtr& spr, bool local);
	static void SprP3dSetLoop(const SprPtr& spr, bool loop);
	static bool SprP3dIsFinished(const SprPtr& spr);
	static void SprP3dUpdate(const SprPtr& spr, float dt);

	// proxy
	static void SprGetProxyChildren(const SprConstPtr& spr, std::vector<ActorPtr>& actors);
	
	// dtex
	static void SetDTexEnable(const SprPtr& spr, bool enable);
	static void SetDTexForceCached(const SprPtr& spr, bool cache);
	static void SetDTexForceCachedDirty(const SprPtr& spr, bool dirty);
	
	/************************************************************************/
	/* actor                                                                */
	/************************************************************************/	

	static bool ActorGetAABB(const ActorPtr& actor, sm::rect& aabb);

 	static bool ActorGetPos(const ActorPtr& actor, sm::vec2& pos);
	static void ActorSetPos(ActorPtr& actor, const sm::vec2& pos);
	static bool ActorGetAngle(const ActorPtr& actor, float& angle);
 	static void ActorSetAngle(ActorPtr& actor, float angle);
 	static bool ActorGetScale(const ActorPtr& actor, sm::vec2& scale);
	static void ActorSetScale(ActorPtr& actor, const sm::vec2& scale);

	static bool ActorGetWorldPos(const ActorPtr& actor, sm::vec2& pos);
	static bool ActorGetWorldAngle(const ActorPtr& actor, float& angle);
	static bool ActorGetWorldScale(const ActorPtr& actor, sm::vec2& scale);

	static bool ActorGetVisible(const ActorPtr& actor, bool& visible);
	static void ActorSetVisible(ActorPtr& actor, bool visible);
	static bool ActorGetEditable(const ActorPtr& actor, bool& editable);
	static void ActorSetEditable(ActorPtr& actor, bool editable);

	static bool ActorGetColMul(const ActorPtr& actor, uint32_t& mul);
	static void ActorSetColMul(ActorPtr& actor, uint32_t mul);
	static bool ActorGetColAdd(const ActorPtr& actor, uint32_t& add);
	static void ActorSetColAdd(ActorPtr& actor, uint32_t add);
	static bool ActorGetColMap(const ActorPtr& actor, uint32_t& rmap, uint32_t& gmap, uint32_t& bmap);
	static void ActorSetColMap(ActorPtr& actor, uint32_t rmap, uint32_t gmap, uint32_t bmap);

	static void ActorSetFilter(ActorPtr& actor, int mode);

	// anim
	static bool ActorGetFrame(const ActorPtr& actor, int& frame);
	static void ActorSetFrame(ActorPtr& actor, int frame);

	static bool ActorGetComponentCount(const ActorPtr& actor, int& count);

	// complex
	static void ActorSetAction(ActorPtr& actor, const std::string& action);
	static bool ActorGetScissor(const ActorPtr& actor, sm::rect& rect);
	static void ActorSetScissor(ActorPtr& actor, const sm::rect& rect);

	// textbox
	static bool ActorGetText(const ActorPtr& actor, std::string& text);
	static void ActorSetText(ActorPtr& actor, const std::string& text);

	// scale9
	static void ActorScale9Resize(ActorPtr& actor, int w, int h);

}; // ProxyHelper

}

#endif // _SPRITE2_PROXY_HELPER_H_
