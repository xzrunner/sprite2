#ifndef _SPRITE2_TRAIL_SPRITE_H_
#define _SPRITE2_TRAIL_SPRITE_H_

#include "S2_Sprite.h"

struct t2d_emitter;

namespace s2
{

class TrailSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TrailSprite();
	TrailSprite(const TrailSprite& spr);
	TrailSprite& operator = (const TrailSprite& spr);
	TrailSprite(Symbol* sym, uint32_t id = -1);
	virtual ~TrailSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TrailSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame);

	void Draw(const RenderParams& rp) const;

	bool IsLocal() const { return m_local; }
	void SetLocal(bool local) { m_local = local; }

	void SetInP3d() { m_in_p3d = true; }
	
protected:
	t2d_emitter* m_et;

	bool m_local;
	bool m_in_p3d;
	
	VI_DUMMY_FUNC

}; // TrailSprite

}

#endif // _SPRITE2_TRAIL_SPRITE_H_