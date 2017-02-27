#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "S2_Symbol.h"

#include <ps_3d.h>

#include <stdint.h>

namespace s2
{

class Particle3dSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Particle3dSymbol();
	Particle3dSymbol(uint32_t id);
	virtual ~Particle3dSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	void SetEmitterCfg(p3d_emitter_cfg* cfg);
	const p3d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	p3d_emitter* GetEmitter() { return m_et; }

	bool IsLoop() const { return m_loop; }
	void SetLoop(bool loop) { m_loop = loop; }

	bool IsLocal() const { return m_local; }
	void SetLocal(bool local) { m_local = local; }

protected:
	p3d_emitter_cfg* m_et_cfg;

	p3d_emitter* m_et;

	bool m_loop, m_local;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_