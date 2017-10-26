#ifndef _SPRITE2_PARTICLE2D_SPRITE_H_
#define _SPRITE2_PARTICLE2D_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/Particle2d.h"
#include "sprite2/RenderReturn.h"

struct p2d_emitter;

namespace s2
{

class RenderParams;

class Particle2dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle2dSprite();
	Particle2dSprite(const Particle2dSprite& spr);
	Particle2dSprite& operator = (const Particle2dSprite& spr);
	Particle2dSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~Particle2dSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);

	RenderReturn Draw(const RenderParams& rp) const;

	void SetMatrix(const S2_MAT& mat) const { m_mat = mat; }

	bool IsLoop() const;
	void SetLoop(bool loop);

	bool IsLocal() const;
	void SetLocal(bool local);

protected:
	p2d_emitter* m_et;

	mutable S2_MAT m_mat;

	SPRITE_CLONE_FUNC(Particle2dSprite)

	VI_DUMMY_FUNC

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE2D_SPRITE_H_