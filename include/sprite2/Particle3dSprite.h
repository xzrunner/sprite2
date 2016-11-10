#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"
#include "Particle3d.h"

struct p3d_sprite;

namespace s2
{

class Particle3dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle3dSprite();
	Particle3dSprite(const Particle3dSprite& spr);
	Particle3dSprite& operator = (const Particle3dSprite& spr);
	Particle3dSprite(Symbol* sym);
	virtual ~Particle3dSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle3dSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg);
	virtual bool Update(const RenderParams& params);

	void Draw(const RenderParams& params) const;

	void SetOuterMatrix(const sm::mat4& mat) const;

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

	void SetReuse(bool reuse);

	void SetLoop(bool loop);

	bool IsLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	float GetStartRadius() const { return m_start_radius; }
	void SetStartRadius(float radius) { m_start_radius = radius; }

	p3d_sprite* GetP3dSpr() const { return m_spr; }

protected:
	void CreateSpr();

protected:
	p3d_sprite* m_spr;

	bool m_alone;
	bool m_reuse;

	mutable P3dRenderParams m_rp;

	float m_start_radius;

	VI_DUMMY_FUNC

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_