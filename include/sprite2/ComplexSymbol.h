#ifndef _SPRITE2_COMPLEX_SYMBOL_H_
#define _SPRITE2_COMPLEX_SYMBOL_H_

#include "sprite2/Symbol.h"

#include <cu/cu_stl.h>

namespace s2
{

class ComplexSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ComplexSymbol();
	ComplexSymbol(uint32_t id);
	virtual ~ComplexSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override;
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN
	virtual bool Update(const UpdateParams& up, float time) override;

	const CU_VEC<SprPtr>& GetActionChildren(int action) const;
	const CU_VEC<SprPtr>& GetAllChildren() const { return m_children; }

	const sm::rect& GetScissor() const { return m_scissor; }
	void SetScissor(const sm::rect& scissor);

	int GetActionIdx(const CU_STR& name) const;

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	// todo up action
	bool Add(const SprPtr& spr, int idx = -1);
	bool Remove(const Sprite& spr);
//	bool Change(const SprTreePath& path, const CU_STR& name, Sprite* dst);
	bool Clear();
	bool ResetOrder(const Sprite& spr, bool up);
	bool ResetOrderMost(const Sprite& spr, bool up);
	bool Sort(CU_VEC<SprPtr>& sprs);

public:
	struct Action
	{
		CU_STR name;
		CU_VEC<SprPtr> sprs;
	};

	const auto& GetActions() const { return m_actions; }
	void SetActions(const CU_VEC<Action>& actions) { m_actions = actions; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

	int GetAction(const Sprite* spr, const Actor* actor) const;

protected:
	CU_VEC<SprPtr> m_children;

	CU_VEC<Action> m_actions;

	sm::rect m_scissor;

	mutable sm::rect m_aabb;

}; // ComplexSymbol

}

#endif // _SPRITE2_COMPLEX_SYMBOL_H_