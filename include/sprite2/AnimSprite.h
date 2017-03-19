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
	virtual void OnMessage(Message msg, const SprTreePath& path);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& parent_path, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;
	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	const AnimCurr& GetAnimCurr(const SprTreePath& path) const;

	void SetLoop(bool loop) { m_loop = loop; }
	void SetInterval(float dt) { m_interval = dt; }

	void SetFPS(int fps) { m_fps = fps; }

	void SetStartRandom(bool random, const SprTreePath& path);

	int  GetFrame(const SprTreePath& path) const;

	void SetActive(bool active, const SprTreePath& path);

private:
	void RandomStartTime(const SprTreePath& path);

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