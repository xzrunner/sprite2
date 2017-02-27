#include "MeshSymbol.h"
#include "SymType.h"
#include "MeshSprite.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "DrawMesh.h"
#include "DrawNode.h"
#include "Mesh.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

MeshSymbol::MeshSymbol()
	: m_mesh(NULL)
	, m_pause(false)
{
}

MeshSymbol::MeshSymbol(uint32_t id)
	: Symbol(id)
	, m_mesh(NULL)
	, m_pause(false)
{
}

MeshSymbol::~MeshSymbol()
{
	if (m_mesh) {
		m_mesh->RemoveReference();
	}
}

int MeshSymbol::Type() const 
{ 
	return SYM_MESH; 
}

void MeshSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_mesh) {
		return;
	}

	RenderParams rp_child;
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

// 	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 	mgr->SetShader(sl::SPRITE2);
// 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
// 	shader->SetColor(p.color.GetMul().ToABGR(), p.color.GetAdd().ToABGR());
// 	shader->SetColorMap(p.color.GetMapR().ToABGR(), p.color.GetMapG().ToABGR(), p.color.GetMapB().ToABGR());

	const MeshSprite* mesh_spr = VI_DOWNCASTING<const MeshSprite*>(spr);
 	if (mesh_spr) {
		const pm::MeshTransform& mtrans = mesh_spr->GetMeshTrans();
		m_mesh->LoadFromTransform(mtrans);
 	}

 	if (mesh_spr && mesh_spr->OnlyDrawBound()) {
 		DrawMesh::DrawInfoXY(m_mesh, &rp_child.mt);
 	} else {
 		DrawMesh::DrawTexture(m_mesh, rp_child, mesh_spr ? mesh_spr->GetBaseSym() : NULL);
 	}
 
//  	if (!m_pause && mesh_spr)
//  	{
//  		sm::vec2 spd = mesh_spr->GetSpeed();
//  		if (spd.x != 0 || spd.y != 0) {
//  			m_mesh->OffsetUV(spd.x, spd.y);
//  		}
//  	}
}

bool MeshSymbol::Update(const RenderParams& rp, float time)
{
 	if (m_mesh) {
 		return const_cast<Symbol*>(m_mesh->GetBaseSymbol())->Update(rp, time);
 	} else {
		return false;
	}
}

sm::rect MeshSymbol::GetBounding(const Sprite* spr) const
{
 	if (m_mesh) {
 		return m_mesh->GetRegion();
 	} else {
		return sm::rect();
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

void MeshSymbol::SetMesh(Mesh* mesh)
{
	cu::RefCountObjAssign(m_mesh, mesh);
}

}