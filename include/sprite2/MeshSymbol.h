#ifndef _SPRITE2_MESH_SYMBOL_H_
#define _SPRITE2_MESH_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/Mesh.h"

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN
	virtual bool Update(const UpdateParams& up, float time) override;

	void UpdateMesh(const rg_skeleton_pose* sk_pose);
	void UpdateMesh(const rg_tl_deform_state* deform_state, const float* vertices);

	const std::unique_ptr<Mesh>& GetMesh() const { return m_mesh; }
	std::unique_ptr<Mesh>& GetMesh() { return m_mesh; }
	void SetMesh(std::unique_ptr<Mesh>& mesh) { m_mesh = std::move(mesh); }

	void SetPause(bool pause) { m_pause = pause; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;

private:
	bool m_pause;

}; // MeshSymbol

}

#endif // _SPRITE2_MESH_SYMBOL_H_