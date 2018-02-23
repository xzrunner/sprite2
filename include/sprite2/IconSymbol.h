#ifndef _SPRITE2_ICON_SYMBOL_H_
#define _SPRITE2_ICON_SYMBOL_H_

#include "sprite2/Icon.h"
#include "sprite2/Symbol.h"

#include <stdint.h>

namespace s2
{

class IconSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	IconSymbol();
	IconSymbol(uint32_t id);
	IconSymbol(const IconSymbol&);
	IconSymbol& operator = (const IconSymbol&);
	virtual ~IconSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

	void SetIcon(std::unique_ptr<Icon>& icon) { m_icon = std::move(icon); }
	const std::unique_ptr<Icon>& GetIcon() const { return m_icon; }
	std::unique_ptr<Icon>& GetIcon() { return m_icon; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

protected:
	std::unique_ptr<Icon> m_icon = nullptr;

}; // IconSymbol

}

#endif // _SPRITE2_ICON_SYMBOL_H_