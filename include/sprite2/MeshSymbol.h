#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

struct rg_skeleton_pose;
struct rg_tl_deform_state;

namespace s2
{

class Mesh;

class MeshSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	MeshSymbol();
	MeshSymbol(uint32_t id);
	virtual ~MeshSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual RenderReturn DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const UpdateParams& up, float time);

	void UpdateMesh(const rg_skeleton_pose* sk_pose);
	void UpdateMesh(const rg_tl_deform_state* deform_state, const float* vertices);

	const Mesh* GetMesh() const { return m_mesh; }
	Mesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

	void SetPause(bool pause) { m_pause = pause; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	Mesh* m_mesh;

private:
	bool m_pause;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_