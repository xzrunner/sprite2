#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/AnimState.h"
#include "sprite2/AnimCurr.h"

#include <cu/cu_stl.h>

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
	virtual void OnMessage(const UpdateParams& up, Message msg) override;
	virtual bool Update(const UpdateParams& up) override;
	virtual bool NeedAutoUpdate(const Actor* actor) const override { return true; }
	virtual bool AutoUpdate(const Actor* actor) override;
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const override;
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const override;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const override;
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const override;

	AnimCurr& GetOriginCurr(const Actor* actor);
	AnimCurr& GetUpdateCurr(const Actor* actor);
	AnimCurr& GetDrawCurr(const Actor* actor);

	const AnimCurr& GetOriginCurr(const Actor* actor) const;
	const AnimCurr& GetUpdateCurr(const Actor* actor) const;
	const AnimCurr& GetDrawCurr(const Actor* actor) const;

	const AnimCurrPtr& GetOriginCurrPtr(const Actor* actor) const;

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(const UpdateParams& up, bool random);

	int  GetFrame(const Actor* actor) const;
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

	S2_SPR_CLONE_FUNC(AnimSprite)

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_