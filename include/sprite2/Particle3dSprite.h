#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"
#include "Particle3d.h"

namespace s2
{

class RenderParams;
class Particle3dEmitter;

class Particle3dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle3dSprite();
	Particle3dSprite(const Particle3dSprite& spr);
	Particle3dSprite& operator = (const Particle3dSprite& spr);
	Particle3dSprite(Symbol* sym, uint32_t id = -1);
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
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual bool SetFrame(const UpdateParams& up, int frame, bool force = false);

	void Draw(const RenderParams& rp) const;

	void SetPrevMat(const S2_MAT& mat) const;

	bool IsLoop() const { return m_loop; }
	void SetLoop(bool loop);

	bool IsLocal() const { return m_local; }
	void SetLocal(bool local);

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

//	void SetReuse(bool reuse);

	float GetStartRadius() const { return m_start_radius; }
	void SetStartRadius(float radius) { m_start_radius = radius; }

protected:
	void CreateSpr();

protected:
	Particle3dEmitter* m_et;

	bool m_loop;
	bool m_local;
	bool m_alone;
	bool m_reuse;

	float m_start_radius;

	VI_DUMMY_FUNC

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_