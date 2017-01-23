#ifndef _SPRITE2_ICON_SYMBOL_H_
#define _SPRITE2_ICON_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Icon;

class IconSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	IconSymbol();
	IconSymbol(uint32_t id);
	virtual ~IconSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymVisitor& visitor) {}
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	void SetIcon(Icon* icon);
	const Icon* GetIcon() const { return m_icon; }
	Icon* GetIcon() { return m_icon; }

protected:
	Icon* m_icon;

}; // IconSymbol

}

#endif // _SPRITE2_ICON_SYMBOL_H_