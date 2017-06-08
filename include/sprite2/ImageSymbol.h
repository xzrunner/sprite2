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
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual void Flattening(const FlattenParams& fp, Flatten& ft) const;

	// todo: only for scale9 layout
	virtual sm::vec2 GetNoTrimedSize() const;

	virtual bool QueryTexcoords(bool use_dtex, float* texcoords, int& tex_id) const = 0;
	virtual bool OnQueryTexcoordsFail() const = 0;

	void InitTex(Texture* tex, const sm::i16_rect& region);

	const Texture* GetTexture() const { return m_tex; }

	const sm::i16_rect& GetRegion() const { return m_region; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

#ifdef S2_DEBUG
	virtual bool IsProxyImg() const = 0;
#endif // S2_DEBUG

private:
	void DrawBlend(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const;
	void DrawOrtho(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const;
	void DrawPseudo3D(const RenderParams& rp, sm::vec2* vertices, float* texcoords, int tex_id) const;

protected:
	Texture* m_tex;

	sm::i16_rect m_region;
	sm::rect     m_size;
	bool         m_rotate;

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_