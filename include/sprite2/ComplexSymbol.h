#ifndef _SPRITE2_COMPLEX_SYMBOL_H_
#define _SPRITE2_COMPLEX_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>
#include <string>

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor);
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;
	virtual bool Update(const UpdateParams& up, float time);

	const std::vector<Sprite*>& GetActionChildren(int action) const;
	const std::vector<Sprite*>& GetAllChildren() const { return m_children; }

	const sm::rect& GetScissor() const { return m_scissor; }
	void SetScissor(const sm::rect& scissor);

	int GetActionIdx(const std::string& name) const;

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	// todo up action
	bool Add(Sprite* spr, int idx = -1);
	bool Remove(Sprite* spr);
//	bool Change(const SprTreePath& path, const std::string& name, Sprite* dst);
	bool Clear();
	bool ResetOrder(const Sprite* spr, bool up);
	bool ResetOrderMost(const Sprite* spr, bool up);
	bool Sort(std::vector<Sprite*>& sprs);

public:
	struct Action
	{
		std::string name;
		std::vector<Sprite*> sprs;
	};

	const std::vector<Action>& GetActions() const { return m_actions; }
	void SetActions(const std::vector<Action>& actions) { m_actions = actions; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

private:
	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

	int GetAction(const Sprite* spr, const Actor* actor) const;

protected:
	std::vector<Sprite*> m_children;

	std::vector<Action> m_actions;

	sm::rect m_scissor;

	mutable sm::rect m_aabb;

}; // ComplexSymbol

}

#endif // _SPRITE2_COMPLEX_SYMBOL_H_