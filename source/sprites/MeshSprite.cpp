#include "MeshSprite.h"
#include "MeshSymbol.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite() 
	: m_only_draw_bound(false)
// 	: m_base(NULL)
// 	, m_only_draw_bound(false)
{
//	m_speed.set(0, -0.01f);
}

MeshSprite::MeshSprite(const MeshSprite& mesh)
	: Sprite(mesh)
	, m_speed(mesh.m_speed)
//	, m_trans(mesh.m_trans)
	, m_only_draw_bound(mesh.m_only_draw_bound)
{
// 	if (m_base = mesh.m_base) {
// 		m_base->AddReference();
// 	}
}

MeshSprite& MeshSprite::operator = (const MeshSprite& mesh)
{
	Sprite::operator = (mesh);
	m_speed = mesh.m_speed;
//	m_trans = mesh.m_trans;
//	cu::RefCountObjAssign(m_base, const_cast<const Symbol*>(mesh.m_base));
	m_only_draw_bound = mesh.m_only_draw_bound;
	return *this;
}

MeshSprite::MeshSprite(Symbol* sym, uint32_t id) 
	: Sprite(sym, id) 
	, m_only_draw_bound(false)
{
	MeshSymbol* mesh_sym = VI_DOWNCASTING<MeshSymbol*>(sym);
// 	if (m_base = mesh_sym->m_base) {
// 		m_base->AddReference();
// 	}

//	m_speed.set(0, -0.01f);

//	m_trans.LoadFromMesh(mesh);
}

MeshSprite::~MeshSprite()
{
// 	if (m_base) {
// 		m_base->RemoveReference();
// 	}
}

MeshSprite* MeshSprite::Clone() const
{
	return new MeshSprite(*this);
}

bool MeshSprite::Update(const RenderParams& params) 
{ 
// 	if (m_base) {
// 		return const_cast<Symbol*>(m_base)->Update(params, 0);
// 	} else {
// 		Mesh* mesh = VI_DOWNCASTING<MeshSymbol*>(m_sym)->GetMesh();
// 		return const_cast<Symbol*>(mesh->GetBaseSymbol())->Update(params, 0);
// 	}

	return false;
}

void MeshSprite::Lerp(const MeshSprite* begin, const MeshSprite* end, float process)
{
//	m_trans.Lerp(begin->GetMeshTrans(), end->GetMeshTrans(), process);
}

// void MeshSprite::SetBaseSym(const Symbol* sym) 
// { 
// 	cu::RefCountObjAssign(m_base, sym); 
// }

}