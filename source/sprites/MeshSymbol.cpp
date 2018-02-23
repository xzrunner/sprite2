#include "sprite2/MeshSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/MeshSprite.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawMesh.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

MeshSymbol::MeshSymbol()
	: m_pause(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS
}

MeshSymbol::MeshSymbol(uint32_t id)
	: Symbol(id)
	, m_pause(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS
}

MeshSymbol::~MeshSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS
}

int MeshSymbol::Type() const 
{ 
	return SYM_MESH; 
}

pt2::RenderReturn MeshSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_MESH);
	StatSymDraw::DrawCostCP cp(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS

	if (!m_mesh) {
		return pt2::RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

// 	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 	mgr->SetShader(sl::SPRITE2);
// 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
// 	shader->SetColor(p.color.GetMulABGR(), p.color.GetAddABGR());
// 	shader->SetColorMap(p.color.GetMapRABGR(), p.color.GetMapGABGR(), p.color.GetMapBABGR());

	auto mesh_spr = S2_VI_DOWN_CAST<const MeshSprite*>(spr);
 	if (mesh_spr) {
		const pm::MeshTransform& mtrans = mesh_spr->GetMeshTrans();
		m_mesh->LoadFromTransform(mtrans);
 	}

	pt2::RenderReturn ret = pt2::RENDER_OK;
 	if (mesh_spr && mesh_spr->OnlyDrawBound()) {
 		ret = DrawMesh::DrawInfoXY(dlist, *m_mesh, &rp_child->mt);
 	} else {
 		ret = DrawMesh::DrawTexture(dlist, *m_mesh, *rp_child, mesh_spr ? mesh_spr->GetBaseSym() : nullptr);
 	}
 
//  	if (!m_pause && mesh_spr)
//  	{
//  		sm::vec2 spd = mesh_spr->GetSpeed();
//  		if (spd.x != 0 || spd.y != 0) {
//  			m_mesh->OffsetUV(spd.x, spd.y);
//  		}
//  	}

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn MeshSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!m_mesh) {
		return pt2::RENDER_NO_DATA;
	}

	auto mesh_spr = S2_VI_DOWN_CAST<const MeshSprite*>(spr);
	if (mesh_spr) {
		const pm::MeshTransform& mtrans = mesh_spr->GetMeshTrans();
		m_mesh->LoadFromTransform(mtrans);
	}

	pt2::RenderReturn ret = pt2::RENDER_OK;
	if (mesh_spr && mesh_spr->OnlyDrawBound()) {
		ret = DrawMesh::DrawInfoXY(dlist, *m_mesh, &rp.mt);
	} else {
		ret = DrawMesh::DrawTexture(dlist, *m_mesh, rp, mesh_spr ? mesh_spr->GetBaseSym() : nullptr);
	}
	return ret;
}
#endif // S2_DISABLE_FLATTEN

bool MeshSymbol::Update(const UpdateParams& up, float time)
{
 	if (m_mesh) {
 		return std::const_pointer_cast<Symbol>(m_mesh->GetBaseSymbol())->Update(up, time);
 	} else {
		return false;
	}
}

void MeshSymbol::UpdateMesh(const rg_skeleton_pose* sk_pose)
{
 	if (m_mesh) {
 		m_mesh->Update(sk_pose);
 	}
}

void MeshSymbol::UpdateMesh(const rg_tl_deform_state* deform_state, const float* vertices)
{
 	if (m_mesh) {
 		m_mesh->Update(deform_state, vertices);
 	}
}

sm::rect MeshSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_mesh) {
		return m_mesh->GetRegion();
	} else {
		return sm::rect(); // empty
	}
}

}