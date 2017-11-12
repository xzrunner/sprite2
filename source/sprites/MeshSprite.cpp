#include "sprite2/MeshSprite.h"
#include "sprite2/MeshSymbol.h"
#include "sprite2/Mesh.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/SymbolVisitor.h"
#include "sprite2/Actor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite() 
	: m_only_draw_bound(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS

//	m_speed.set(0, -0.01f);
}

MeshSprite::MeshSprite(const MeshSprite& mesh)
	: Sprite(mesh)
	, m_speed(mesh.m_speed)
	, m_trans(mesh.m_trans)
	, m_base(mesh.m_base)
	, m_only_draw_bound(mesh.m_only_draw_bound)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS
}

MeshSprite& MeshSprite::operator = (const MeshSprite& mesh)
{
	Sprite::operator = (mesh);
	m_speed = mesh.m_speed;
	m_trans = mesh.m_trans;
	m_base = mesh.m_base;
	m_only_draw_bound = mesh.m_only_draw_bound;
	return *this;
}

MeshSprite::MeshSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id) 
	, m_only_draw_bound(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS

	const std::unique_ptr<Mesh>& mesh = S2_VI_PTR_DOWN_CAST<const MeshSymbol>(sym)->GetMesh();
	m_base = mesh->GetBaseSymbol();

//	m_speed.set(0, -0.01f);

	mesh->StoreToTransforom(m_trans);
}

MeshSprite::~MeshSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_MESH);
#endif // S2_DISABLE_STATISTICS
}

void MeshSprite::OnMessage(const UpdateParams& up, Message msg)
{
	
}

bool MeshSprite::Update(const UpdateParams& up) 
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	UpdateParams up_child(up);
	up_child.Push(this);
	bool ret;
	if (m_base) {
		ret = std::const_pointer_cast<Symbol>(m_base)->Update(up_child, 0);
	} else {
		const auto& mesh = S2_VI_PTR_DOWN_CAST<MeshSymbol>(m_sym)->GetMesh();
		ret = std::const_pointer_cast<Symbol>(mesh->GetBaseSymbol())->Update(up_child, 0);
	}
	return ret;
}

SprPtr MeshSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	class FetchVisitor : public SymbolVisitor
	{
	public:
		FetchVisitor(int name) 
			: m_name(name)
		{}

		virtual void Visit(const SprPtr& spr) const override
		{
			if (!m_spr && spr->GetName() == m_name) {
				m_spr = spr;
			}
		}

		SprPtr GetResult() { return m_spr; }
		
	private:
		int m_name;
		mutable SprPtr m_spr = nullptr;

	}; // FetchVisitor

	FetchVisitor visitor(name);
	if (m_base) {
		std::const_pointer_cast<Symbol>(m_base)->Traverse(visitor);
	} else {
		const auto& mesh = S2_VI_PTR_DOWN_CAST<MeshSymbol>(m_sym)->GetMesh();
		std::const_pointer_cast<Symbol>(mesh->GetBaseSymbol())->Traverse(visitor);
	}

	return visitor.GetResult();
}

void MeshSprite::Lerp(const MeshSprite& begin, const MeshSprite& end, float process)
{
	m_trans.Lerp(begin.GetMeshTrans(), end.GetMeshTrans(), process);
}

}