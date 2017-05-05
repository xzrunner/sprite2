#ifndef _SPRITE2_PROXY_HELPER_H_
#define _SPRITE2_PROXY_HELPER_H_

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>
#include <string>

namespace s2
{

class Sprite;
class Actor;

class ProxyHelper
{
public:
	static Sprite* BuildGroup(const std::vector<std::pair<const Actor*, Sprite*> >& items);

	/************************************************************************/
	/* sprite                                                               */
	/************************************************************************/

	static bool SprGetPos(const Sprite* spr, sm::vec2& pos);
	static bool SprGetAngle(const Sprite* spr, float& angle);
	static bool SprGetScale(const Sprite* spr, sm::vec2& scale);

	static bool SprGetSymID(const Sprite* spr, int& id);
	static bool SprGetSymType(const Sprite* spr, int& type);

	static bool SprPointTest(const Sprite* spr, const sm::vec2& pos);
	static const Actor* SprPointQuery(const Sprite* spr, const sm::vec2& pos, float mat[6]);

	static bool SprGetForceUpdate(const Sprite* spr, bool& force);
	static void SprSetForceUpdate(Sprite* spr, bool force);

	// anim
	static bool SprGetFrameCount(const Sprite* spr, int& count);

	// complex
	static bool SprHasAction(const Sprite* spr, const std::string& action);
	static bool SprGetScissor(const Sprite* spr, sm::rect& rect);
	static void SprSetScissor(Sprite* spr, const sm::rect& rect);

	// textbox
	static void SprTextboxResetTime(Sprite* spr);

	// anim2
	static void SprAnim2SetStaticTime(Sprite* spr, int time);

	// particle3d
	static void SprP3dSetLocal(Sprite* spr, bool local);
	static void SprP3dSetLoop(Sprite* spr, bool loop);
	static bool SprP3dIsFinished(const Sprite* spr);
	static void SprP3dUpdate(Sprite* spr, float dt);

	// proxy
	static void SprGetProxyChildren(const Sprite* spr, std::vector<const Actor*>& actors);
	
	// dtex
	static void SetDTexEnable(Sprite* spr, bool enable);
	static void SetDTexForceCached(Sprite* spr, bool cache);
	static void SetDTexForceCachedDirty(Sprite* spr, bool dirty);
	
	/************************************************************************/
	/* actor                                                                */
	/************************************************************************/	

	static bool ActorGetAABB(const Actor* actor, sm::rect& aabb);

 	static bool ActorGetPos(const Actor* actor, sm::vec2& pos);
	static void ActorSetPos(Actor* actor, const sm::vec2& pos);
	static bool ActorGetAngle(const Actor* actor, float& angle);
 	static void ActorSetAngle(Actor* actor, float angle);
 	static bool ActorGetScale(const Actor* actor, sm::vec2& scale);
	static void ActorSetScale(Actor* actor, const sm::vec2& scale);

	static bool ActorGetWorldPos(const Actor* actor, sm::vec2& pos);
	static bool ActorGetWorldAngle(const Actor* actor, float& angle);
	static bool ActorGetWorldScale(const Actor* actor, sm::vec2& scale);

	static bool ActorGetVisible(const Actor* actor, bool& visible);
	static void ActorSetVisible(Actor* actor, bool visible);
	static bool ActorGetEditable(const Actor* actor, bool& editable);
	static void ActorSetEditable(Actor* actor, bool editable);

	static bool ActorGetColMul(const Actor* actor, uint32_t& mul);
	static void ActorSetColMul(Actor* actor, uint32_t mul);
	static bool ActorGetColAdd(const Actor* actor, uint32_t& add);
	static void ActorSetColAdd(Actor* actor, uint32_t add);
	static bool ActorGetColMap(const Actor* actor, uint32_t& rmap, uint32_t& gmap, uint32_t& bmap);
	static void ActorSetColMap(Actor* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap);

	static void ActorSetFilter(Actor* actor, int mode);

	// anim
	static bool ActorGetFrame(const Actor* actor, int& frame);
	static void ActorSetFrame(Actor* actor, int frame);

	static bool ActorGetComponentCount(const Actor* actor, int& count);

	// complex
	static void ActorSetAction(Actor* actor, const std::string& action);

	// textbox
	static bool ActorGetText(const Actor* actor, std::string& text);
	static void ActorSetText(Actor* actor, const std::string& text);

	// scale9
	static void ActorScale9Resize(Actor* actor, int w, int h);

}; // ProxyHelper

}

#endif // _SPRITE2_PROXY_HELPER_H_
