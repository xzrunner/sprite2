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
	virtual void Traverse(const SymVisitor& visitor) {}
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual bool Update(const RenderParams& params, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	virtual void UpdateMesh(const rg_skeleton_pose* sk_pose);
	virtual void UpdateMesh(const rg_tl_deform_state* deform_state, const float* vertices);

// 	const Mesh* GetMesh() const { return m_mesh; }
// 	Mesh* GetMesh() { return m_mesh; }
// 	void SetMesh(Mesh* mesh);

	void SetPause(bool pause) { m_pause = pause; }

// protected:
// 	Mesh* m_mesh;

private:
	bool m_pause;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_