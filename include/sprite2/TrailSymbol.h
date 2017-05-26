#ifndef _SPRITE2_TRAIL_SYMBOL_H_
#define _SPRITE2_TRAIL_SYMBOL_H_

#include "S2_Symbol.h"

namespace s2
{

class TrailEmitter;
class TrailEmitterCfg;

class TrailSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TrailSymbol();
	TrailSymbol(uint32_t id);
	virtual ~TrailSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;

	void SetEmitterCfg(const TrailEmitterCfg* cfg);
	const TrailEmitterCfg* GetEmitterCfg() const { return m_et_cfg; }

	TrailEmitter* GetEmitter() const { return m_et; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	const TrailEmitterCfg* m_et_cfg;

	TrailEmitter* m_et;
}; // TrailSymbol

}

#endif // _SPRITE2_TRAIL_SYMBOL_H_