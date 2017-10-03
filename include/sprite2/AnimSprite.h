#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "S2_Sprite.h"
#include "sprite2/AnimState.h"
#include "sprite2/AnimCurr.h"

#include <vector>

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(const SymPtr& sym, uint32_t id = -1);
	AnimSprite(const AnimSprite& spr);
	AnimSprite& operator = (const AnimSprite& spr);
	virtual ~AnimSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual bool NeedAutoUpdate(const ActorConstPtr& actor) const { return true; }
	virtual bool AutoUpdate(const ActorConstPtr& actor);
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	AnimCurr& GetOriginCurr(const ActorConstPtr& actor);
	AnimCurr& GetUpdateCurr(const ActorConstPtr& actor);
	AnimCurr& GetDrawCurr(const ActorConstPtr& actor);

	const AnimCurr& GetOriginCurr(const ActorConstPtr& actor) const;
	const AnimCurr& GetUpdateCurr(const ActorConstPtr& actor) const;
	const AnimCurr& GetDrawCurr(const ActorConstPtr& actor) const;

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(const UpdateParams& up, bool random);

	int  GetFrame(const ActorConstPtr& actor) const;
	bool SetFrame(const UpdateParams& up, int frame);

	void SetActive(bool active, const ActorConstPtr& actor);

private:
	void RandomStartTime(const UpdateParams& up);

protected:
	bool m_loop;
	float m_interval;

	int m_fps;

	bool m_start_random;

	AnimState m_state;

	SPRITE_CLONE_FUNC(AnimSprite)

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_