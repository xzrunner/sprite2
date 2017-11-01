#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/Particle3d.h"

namespace s2
{

class RenderParams;
class Particle3dEmitter;
class P3dEmitterCfg;

class Particle3dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle3dSprite();
	Particle3dSprite(const Particle3dSprite& spr);
	Particle3dSprite& operator = (const Particle3dSprite& spr);
	Particle3dSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~Particle3dSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);

	void SetEmitterMat(const S2_MAT& mat) const;

	bool IsLoop() const { return m_loop; }
	void SetLoop(bool loop);

	bool IsLocal() const { return m_local; }
	void SetLocal(bool local);

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

	auto& GetEmitter() const { return m_et; }
	auto& GetEmitter() { return m_et; }

	enum ReuseType
	{
		REUSE_ALL = 0,
		REUSE_COMMON,
		REUSE_NONE
	};
	void SetReuse(ReuseType reuse);
	ReuseType GetReuse() const { return m_reuse; }

	float GetStartRadius() const { return m_start_radius; }
	void SetStartRadius(float radius) { m_start_radius = radius; }

protected:
	void CreateSpr();

private:
	bool UpdateEmitter(const UpdateParams& up, const std::shared_ptr<Particle3dEmitter>& et);

	void ChangeEmitterBuffer(ReuseType reuse, const std::shared_ptr<const P3dEmitterCfg>& cfg, bool add);
	static void ChangeEmitterBuffer(std::shared_ptr<Particle3dEmitter>& et, const std::shared_ptr<const P3dEmitterCfg>& cfg, bool add);

protected:
	std::shared_ptr<Particle3dEmitter> m_et = nullptr;

	bool m_loop;
	bool m_local;
	bool m_alone;
	ReuseType m_reuse;

	float m_start_radius;

	SPRITE_CLONE_FUNC(Particle3dSprite)

	VI_DUMMY_FUNC

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_