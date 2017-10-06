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
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const { return RENDER_SKIP; }

	void SetEmitterCfg(const std::shared_ptr<const TrailEmitterCfg>& cfg);
	const std::shared_ptr<const TrailEmitterCfg>& GetEmitterCfg() const { return m_et_cfg; }

	const std::unique_ptr<TrailEmitter>& GetEmitter() const { return m_et; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	std::shared_ptr<const TrailEmitterCfg> m_et_cfg;

	std::unique_ptr<TrailEmitter> m_et;

}; // TrailSymbol

}

#endif // _SPRITE2_TRAIL_SYMBOL_H_