#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "S2_Sprite.h"

#include <vector>

namespace s2
{

class AnimCurr;

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(Symbol* sym, uint32_t id = -1);
	AnimSprite(const AnimSprite& spr);
	AnimSprite& operator = (const AnimSprite& spr);
	virtual ~AnimSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual bool NeedAutoUpdate(const Actor* actor) const { return true; }
	virtual bool AutoUpdate(const Actor* actor);
	virtual Sprite* FetchChildByName(int name, const Actor* actor) const;
	virtual Sprite* FetchChildByIdx(int idx, const Actor* actor) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	const AnimCurr* GetAnimCurr(const Actor* actor) const;

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(const UpdateParams& up, bool random);

	int  GetFrame(const Actor* actor) const;
	bool SetFrame(const UpdateParams& up, int frame);

	void SetActive(bool active, const Actor* actor);

private:
	void RandomStartTime(const UpdateParams& up);

protected:
	bool m_loop;
	float m_interval;

	int m_fps;

	bool m_start_random;

	AnimCurr* m_curr;

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_