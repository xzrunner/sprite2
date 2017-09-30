#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "S2_Symbol.h"

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;
	virtual bool Update(const UpdateParams& up, float time);

	void SetEmitterCfg(const P3dEmitterCfg* cfg);
	const P3dEmitterCfg* GetEmitterCfg() const { return m_et_cfg; }

	Particle3dEmitter* GetEmitter() { return m_et; }

	void SetLoop(bool loop) { m_loop = loop; }
	bool IsLoop() const { return m_loop; }

	void SetLocal(bool local) { m_local = local; }
	bool IsLocal() const { return m_local; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

private:
	RenderReturn DrawImpl(const RenderParams& rp, const Sprite* spr) const;

	RenderReturn DrawSymbol(const RenderParams& rp, const Sprite* spr) const;
	RenderReturn DrawEmitter(const RenderParams& rp, const Sprite* spr, 
		const Particle3dEmitter* et) const;

	static bool IsVisible(const RenderParams& rp, const Sprite* spr);

private:
	const P3dEmitterCfg* m_et_cfg;

	Particle3dEmitter* m_et;

	bool m_loop;
	bool m_local;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_