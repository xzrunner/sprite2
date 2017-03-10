#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "s2_config.h"

#include "S2_Sprite.h"
#ifdef S2_ANIM_CURR_V0
#include "AnimCurr.h"
#elif defined S2_ANIM_CURR_V1
#include "AnimCurr2.h"
#elif defined S2_ANIM_CURR_V2
#include "AnimCurr3.h"
#endif

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
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;
	virtual VisitResult TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const;

#ifdef S2_ANIM_CURR_V0
	const AnimCurr& GetAnimCurr() const { return m_curr; }
#elif defined S2_ANIM_CURR_V1
	const AnimCurr2& GetAnimCurr() const { return m_curr; }
#elif defined S2_ANIM_CURR_V2
	const AnimCurr3& GetAnimCurr() const { return m_curr; }
#endif

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(bool random);

	int  GetFrame() const { return m_curr.GetFrame(); }

	void SetActive(bool active);

private:
	void RandomStartTime();

protected:
	bool m_loop;
	float m_interval;

	int m_fps;

	bool m_start_random;

#ifdef S2_ANIM_CURR_V0
	AnimCurr m_curr;
#elif defined S2_ANIM_CURR_V1
	AnimCurr2 m_curr;
#elif defined S2_ANIM_CURR_V2
	AnimCurr3 m_curr;
#endif

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_