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
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const UpdateParams& up, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL) const;

	void SetEmitterCfg(const P3dEmitterCfg* cfg);
	const P3dEmitterCfg* GetEmitterCfg() const { return m_et_cfg; }

	Particle3dEmitter* GetEmitter() { return m_et; }

	void SetLoop(bool loop) { m_loop = loop; }
	bool IsLoop() const { return m_loop; }

	void SetLocal(bool local) { m_local = local; }
	bool IsLocal() const { return m_local; }

private:
	void DrawSymbol(const RenderParams& rp, const Sprite* spr) const;
	void DrawEmitter(const RenderParams& rp, const Sprite* spr, 
		const Particle3dEmitter* et) const;

private:
	const P3dEmitterCfg* m_et_cfg;

	Particle3dEmitter* m_et;

	bool m_loop;
	bool m_local;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_