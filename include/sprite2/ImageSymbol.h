#ifndef _SPRITE2_IMAGE_SYMBOL_H_
#define _SPRITE2_IMAGE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Texture;

class ImageSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ImageSymbol();
	ImageSymbol(uint32_t id);
	virtual ~ImageSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymVisitor& visitor) {}
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	// todo: only for scale9 layout
	virtual sm::vec2 GetNoTrimedSize() const;

	virtual bool QueryTexcoords(const RenderParams& params, float* texcoords, int& texid) const = 0;

	void InitTex(Texture* tex, const sm::i16_rect& region);

	const Texture* GetTexture() const { return m_tex; }

	const sm::i16_rect& GetRegion() const { return m_region; }

private:
	void DrawBlend(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawOrtho(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawPseudo3D(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;

protected:
	Texture* m_tex;

	sm::i16_rect m_region;
	sm::rect     m_size;
	bool         m_rotate;

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_