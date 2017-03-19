#ifndef _SPRITE2_PARTICLE2D_SPRITE_H_
#define _SPRITE2_PARTICLE2D_SPRITE_H_

#include "S2_Sprite.h"
#include "Particle2d.h"

struct p2d_emitter;

namespace s2
{

class Particle2dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle2dSprite();
	Particle2dSprite(const Particle2dSprite& spr);
	Particle2dSprite& operator = (const Particle2dSprite& spr);
	Particle2dSprite(Symbol* sym, uint32_t id = -1);
	virtual ~Particle2dSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle2dSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg, const SprTreePath& path);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& parent_path, bool force = false);

	void Draw(const RenderParams& rp) const;

	void SetMatrix(const S2_MAT& mat) const { m_mat = mat; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

protected:
	p2d_emitter* m_et;

	mutable S2_MAT m_mat;

	VI_DUMMY_FUNC

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE2D_SPRITE_H_