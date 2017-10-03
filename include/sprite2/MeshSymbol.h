#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "S2_Symbol.h"
#include "S2_Mesh.h"

#include <stdint.h>

struct rg_skeleton_pose;
struct rg_tl_deform_state;

namespace s2
{

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
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;
	virtual bool Update(const UpdateParams& up, float time);

	void UpdateMesh(const rg_skeleton_pose* sk_pose);
	void UpdateMesh(const rg_tl_deform_state* deform_state, const float* vertices);

	const std::unique_ptr<Mesh>& GetMesh() const { return m_mesh; }
	std::unique_ptr<Mesh>& GetMesh() { return m_mesh; }
	void SetMesh(std::unique_ptr<Mesh>& mesh) { m_mesh = std::move(mesh); }

	void SetPause(bool pause) { m_pause = pause; }

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

protected:
	std::unique_ptr<Mesh> m_mesh;

private:
	bool m_pause;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_