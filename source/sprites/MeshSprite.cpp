#include "MeshSprite.h"
#include "MeshSymbol.h"
#include "S2_Mesh.h"
#include "UpdateParams.h"
#include "SymbolVisitor.h"
#include "S2_Actor.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite() 
	: m_base(NULL)
	, m_only_draw_bound(false)
{
//	m_speed.set(0, -0.01f);
}

MeshSprite::MeshSprite(const MeshSprite& mesh)
	: Sprite(mesh)
	, m_speed(mesh.m_speed)
	, m_trans(mesh.m_trans)
	, m_base(mesh.m_base)
	, m_only_draw_bound(mesh.m_only_draw_bound)
{
	if (mesh.m_base) {
		mesh.m_base->AddReference();
	}
}

MeshSprite& MeshSprite::operator = (const MeshSprite& mesh)
{
	Sprite::operator = (mesh);
	m_speed = mesh.m_speed;
	m_trans = mesh.m_trans;
	cu::RefCountObjAssign(m_base, mesh.m_base);
	m_only_draw_bound = mesh.m_only_draw_bound;
	return *this;
}

MeshSprite::MeshSprite(Symbol* sym, uint32_t id) 
	: Sprite(sym, id) 
	, m_only_draw_bound(false)
{
	Mesh* mesh = VI_DOWNCASTING<MeshSymbol*>(sym)->GetMesh();
	m_base = mesh->GetBaseSymbol();
	if (m_base) {
		m_base->AddReference();
	}

//	m_speed.set(0, -0.01f);

	mesh->StoreToTransforom(m_trans);
}

MeshSprite::~MeshSprite()
{
	if (m_base) {
		m_base->RemoveReference();
	}
}

MeshSprite* MeshSprite::Clone() const
{
	return new MeshSprite(*this);
}

void MeshSprite::OnMessage(const UpdateParams& up, Message msg)
{
	
}

bool MeshSprite::Update(const UpdateParams& up) 
{
	UpdateParams up_child(up);
	up_child.Push(this);
	if (m_base) {
		return const_cast<Symbol*>(m_base)->Update(up_child, 0);
	} else {
		Mesh* mesh = VI_DOWNCASTING<MeshSymbol*>(m_sym)->GetMesh();
		return const_cast<Symbol*>(mesh->GetBaseSymbol())->Update(up_child, 0);
	}
}

bool MeshSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}
	// todo
	return false;
}

Sprite* MeshSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	class FetchVisitor : public SymbolVisitor
	{
	public:
		FetchVisitor(const std::string& name) 
			: m_name(name)
			, m_spr(NULL) 
		{}

		virtual void Visit(Sprite* spr) const
		{
			if (!m_spr && spr->GetName() == m_name) {
				m_spr = spr;
			}
		}

		Sprite* GetResult() { return m_spr; }
		
	private:
		std::string m_name;
		mutable Sprite* m_spr;

	}; // FetchVisitor

	FetchVisitor visitor(name);
	if (m_base) {
		const_cast<Symbol*>(m_base)->Traverse(visitor);
	} else {
		Mesh* mesh = VI_DOWNCASTING<MeshSymbol*>(m_sym)->GetMesh();
		const_cast<Symbol*>(mesh->GetBaseSymbol())->Traverse(visitor);
	}
	return visitor.GetResult();
}

void MeshSprite::Lerp(const MeshSprite* begin, const MeshSprite* end, float process)
{
	m_trans.Lerp(begin->GetMeshTrans(), end->GetMeshTrans(), process);
}

void MeshSprite::SetBaseSym(const Symbol* sym) 
{ 
	cu::RefCountObjAssign(m_base, sym); 
}

}