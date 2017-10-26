#ifndef _SPRITE2_PARTICLE2D_SYMBOL_H_
#define _SPRITE2_PARTICLE2D_SYMBOL_H_

#include "sprite2/Symbol.h"

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

	const p2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }
	void SetEmitterCfg(const p2d_emitter_cfg* cfg) { m_et_cfg = cfg; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	const p2d_emitter_cfg* m_et_cfg;

}; // Particle2dSymbol

}

#endif // _SPRITE2_PARTICLE2D_SYMBOL_H_