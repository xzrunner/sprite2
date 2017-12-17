#ifndef _SPRITE2_IMAGE_SYMBOL_H_
#define _SPRITE2_IMAGE_SYMBOL_H_

#include "sprite2/Symbol.h"

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

	// todo: only for scale9 layout
	virtual sm::vec2 GetNoTrimedSize() const;

	virtual bool QueryTexcoords(bool use_dtex, float* texcoords, int& tex_id) const = 0;
	virtual void OnQueryTexcoordsFail(cooking::DisplayList* dlist) const = 0;

	void InitTex(const std::shared_ptr<Texture>& tex, const sm::i16_rect& region);

	const std::shared_ptr<Texture>& GetTexture() const { return m_tex; }

	const sm::i16_rect& GetRegion() const { return m_region; }

	bool IsLoaded() const;

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	void DrawBlend(const RenderParams& rp, float* vertices, const float* texcoords, int tex_id) const;
	void DrawOrtho(cooking::DisplayList* dlist, const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const;
	void DrawPseudo3D(cooking::DisplayList* dlist, const RenderParams& rp, const float* vertices, const float* texcoords, int tex_id) const;

	bool CalcVertices(const RenderParams& rp, float* vertices) const;

protected:
	std::shared_ptr<Texture> m_tex = nullptr;

	sm::i16_rect m_region;
	sm::rect     m_size;
	bool         m_rotate;

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_