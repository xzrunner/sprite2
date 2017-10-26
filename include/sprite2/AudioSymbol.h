#ifndef _SPRITE2_AUDIO_SYMBOL_H_
#define _SPRITE2_AUDIO_SYMBOL_H_

#include "sprite2/Symbol.h"

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override { return RENDER_NO_DATA; }
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetSource(const std::shared_ptr<ua::Source>& src) { m_source = src; }
	const std::shared_ptr<ua::Source>& GetSource() { return m_source; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

private:
	std::shared_ptr<ua::Source> m_source;

}; // AudioSymbol

}

#endif // _SPRITE2_AUDIO_SYMBOL_H_