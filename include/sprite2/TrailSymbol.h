#ifndef _SPRITE2_TRAIL_SYMBOL_H_
#define _SPRITE2_TRAIL_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/TrailEmitter.h"

namespace s2
{

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetEmitterCfg(const std::shared_ptr<const TrailEmitterCfg>& cfg);
	const std::shared_ptr<const TrailEmitterCfg>& GetEmitterCfg() const { return m_et_cfg; }

	const std::unique_ptr<TrailEmitter>& GetEmitter() const { return m_et; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	std::shared_ptr<const TrailEmitterCfg> m_et_cfg = nullptr;

	std::unique_ptr<TrailEmitter> m_et = nullptr;

}; // TrailSymbol

}

#endif // _SPRITE2_TRAIL_SYMBOL_H_