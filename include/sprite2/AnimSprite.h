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
	virtual void OnMessage(Message msg, const Actor* actor);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const Actor* actor, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const Actor* actor) const;
	virtual Sprite* FetchChild(int idx, const Actor* actor) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	const AnimCurr& GetAnimCurr(const Actor* actor) const;

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(bool random, const Actor* actor);

	int  GetFrame(const Actor* actor) const;

	void SetActive(bool active, const Actor* actor);

private:
	void RandomStartTime(const Actor* actor);

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