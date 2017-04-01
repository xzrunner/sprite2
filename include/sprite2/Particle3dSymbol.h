#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

struct p3d_emitter_cfg;

namespace s2
{

class Particle3dEmitter;

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
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL) const;

	void SetEmitterCfg(p3d_emitter_cfg* cfg);
	const p3d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	Particle3dEmitter* GetEmitter() { return m_et; }

private:
	p3d_emitter_cfg* m_et_cfg;

	Particle3dEmitter* m_et;

	bool m_loop;
	bool m_local;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_