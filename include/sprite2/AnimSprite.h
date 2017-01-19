#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "AnimCurr2.h"
#include "S2_Sprite.h"

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

	const AnimCurr2& GetAnimCurr() const { return m_curr; }

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

	AnimCurr2 m_curr;

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_