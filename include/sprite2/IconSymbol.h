#ifndef _SPRITE2_ICON_SYMBOL_H_
#define _SPRITE2_ICON_SYMBOL_H_

#include "Icon.h"
#include "S2_Symbol.h"

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;

	void SetIcon(std::unique_ptr<Icon>& icon) { m_icon = std::move(icon); }
	const std::unique_ptr<Icon>& GetIcon() const { return m_icon; }
	std::unique_ptr<Icon>& GetIcon() { return m_icon; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	std::unique_ptr<Icon> m_icon;

}; // IconSymbol

}

#endif // _SPRITE2_ICON_SYMBOL_H_