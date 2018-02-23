#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "sprite2/Symbol.h"

#include <stdint.h>

namespace s2
{

class Particle3dEmitter;
class P3dEmitterCfg;

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN
	virtual bool Update(const UpdateParams& up, float time) override;

	void SetEmitterCfg(const std::shared_ptr<const P3dEmitterCfg>& cfg);
	auto& GetEmitterCfg() const { return m_et_cfg; }

	const std::shared_ptr<Particle3dEmitter>& GetEmitter() const { return m_et; }
	std::shared_ptr<Particle3dEmitter>& GetEmitter() { return m_et; }

	void SetLoop(bool loop) { m_loop = loop; }
	bool IsLoop() const { return m_loop; }

	void SetLocal(bool local) { m_local = local; }
	bool IsLocal() const { return m_local; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	pt2::RenderReturn DrawImpl(const RenderParams& rp, const Sprite* spr) const;

	pt2::RenderReturn DrawSymbol(const RenderParams& rp, const Sprite* spr) const;
	pt2::RenderReturn DrawEmitter(const RenderParams& rp, const Sprite* spr,
		const std::shared_ptr<Particle3dEmitter>& et) const;

	static bool IsVisible(const RenderParams& rp, const Sprite* spr);

private:
	std::shared_ptr<const P3dEmitterCfg> m_et_cfg = nullptr;

	std::shared_ptr<Particle3dEmitter> m_et = nullptr;

	bool m_loop;
	bool m_local;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_