#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "s2_config.h"

#include "S2_Sprite.h"
#ifdef S2_ANIM_CURR_OLD
#include "AnimCurr.h"
#else
#include "AnimCurr2.h"
#endif // S2_ANIM_CURR_OLD

#include <vector>

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg);
	virtual bool Update(const RenderParams& params);
	virtual Sprite* FetchChild(const std::string& name) const;

#ifdef S2_ANIM_CURR_OLD
	const AnimCurr& GetAnimCurr() const { return m_curr; }
#else
	const AnimCurr2& GetAnimCurr() const { return m_curr; }
#endif // S2_ANIM_CURR_OLD

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(bool random);

	void SetFrame(int frame);
	int  GetFrame() const { return m_curr.GetFrame(); }

	void SetActive(bool active);

protected:
	virtual bool TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const;

private:
	void RandomStartTime();

protected:
	bool m_loop;
	float m_interval;

	int m_fps;

	bool m_start_random;

#ifdef S2_ANIM_CURR_OLD
	AnimCurr m_curr;
#else
	AnimCurr2 m_curr;
#endif // S2_ANIM_CURR_OLD

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_