#ifndef _SPRITE2_AUDIO_SYMBOL_H_
#define _SPRITE2_AUDIO_SYMBOL_H_

#include "sprite2/Symbol.h"

namespace s2
{

class AudioSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	AudioSymbol();
	AudioSymbol(uint32_t id);
	virtual ~AudioSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override { return pt2::RENDER_NO_DATA; }
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return pt2::RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

}; // AudioSymbol

}

#endif // _SPRITE2_AUDIO_SYMBOL_H_