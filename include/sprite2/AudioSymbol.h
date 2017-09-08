#ifndef _SPRITE2_AUDIO_SYMBOL_H_
#define _SPRITE2_AUDIO_SYMBOL_H_

#include "S2_Symbol.h"

namespace ua { class Source; }

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const { return RENDER_NO_DATA; }
	virtual RenderReturn DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = NULL) const { return RENDER_NO_DATA; }

	void SetSource(ua::Source* src);
	ua::Source* GetSource() { return m_source; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	ua::Source* m_source;

}; // AudioSymbol

}

#endif // _SPRITE2_AUDIO_SYMBOL_H_