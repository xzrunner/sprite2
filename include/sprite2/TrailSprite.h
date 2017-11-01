#ifndef _SPRITE2_TRAIL_SPRITE_H_
#define _SPRITE2_TRAIL_SPRITE_H_

#include "sprite2/Sprite.h"

namespace s2
{

class RenderParams;
class TrailEmitter;

class TrailSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TrailSprite();
	TrailSprite(const TrailSprite& spr);
	TrailSprite& operator = (const TrailSprite& spr);
	TrailSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~TrailSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);

	void Draw(const RenderParams& rp) const;

	bool IsLocal() const { return m_local; }
	void SetLocal(bool local) { m_local = local; }

	void SetInP3d() { m_in_p3d = true; }
	
protected:
	void CreateSpr();

protected:
	std::unique_ptr<TrailEmitter> m_et = nullptr;

	bool m_local;
	bool m_in_p3d;

	SPRITE_CLONE_FUNC(TrailSprite)
	
	VI_DUMMY_FUNC

}; // TrailSprite

}

#endif // _SPRITE2_TRAIL_SPRITE_H_