#ifndef _SPRITE2_PARTICLE2D_SYMBOL_H_
#define _SPRITE2_PARTICLE2D_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

struct p2d_emitter_cfg;

namespace s2
{

class Particle2dSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Particle2dSymbol();
	Particle2dSymbol(uint32_t id);
	virtual ~Particle2dSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

	const p2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }
	void SetEmitterCfg(const p2d_emitter_cfg* cfg) { m_et_cfg = cfg; }

protected:
	const p2d_emitter_cfg* m_et_cfg;

}; // Particle2dSymbol

}

#endif // _SPRITE2_PARTICLE2D_SYMBOL_H_