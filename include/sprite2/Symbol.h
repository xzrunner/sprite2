#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"
#ifdef S2_SYMBOL_COMPONENT
#include "sprite2/ComponentsMgr.h"
#endif // S2_SYMBOL_COMPONENT

#include <cu/uncopyable.h>
#include <SM_Rect.h>
#include <painting2/RenderReturn.h>

#include <memory>

#include <stdint.h>
#include <stddef.h>

namespace cooking { class DisplayList; }
namespace ft { class FTList; }

namespace s2
{

class RenderParams;
class UpdateParams;
class Sprite;
class SymbolVisitor;
class Actor;

#ifdef S2_SYMBOL_COMPONENT
class Symbol : private cu::Uncopyable, public ComponentsMgr<8>
#else
class Symbol : private cu::Uncopyable
#endif // S2_SYMBOL_COMPONENT
{
public:
	Symbol();
	Symbol(uint32_t id);
	virtual ~Symbol();
	virtual int Type() const = 0;
	virtual void Traverse(const SymbolVisitor& visitor) = 0;
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const = 0;
#ifdef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const final { return pt2::RENDER_SKIP; }
#else
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const = 0;
#endif // S2_DISABLE_FLATTEN
	virtual bool Update(const UpdateParams& up, float time) { return false; }

	int GetID() const { return m_id; }

	sm::rect GetBounding(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

	static int GetAllSymCount();

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const = 0;

private:
	uint32_t m_id;

}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_