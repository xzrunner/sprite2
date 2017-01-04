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
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	// todo: only for scale9 layout
	virtual sm::vec2 GetNoTrimedSize() const;

	virtual void QueryTexcoords(float* texcoords, int& texid) const = 0;

	void InitTex(Texture* tex, const sm::i16_rect& quad, const sm::vec2& offset);

	const Texture* GetTexture() const { return m_tex; }

protected:
	virtual void Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const = 0;
	virtual bool IsOrthoCam() const = 0;
	virtual void GetScreenSize(int& w, int& h) const = 0;
	virtual float GetP3dCamAngle() const = 0;
	virtual int GetScreenCacheTexid() const = 0;

private:
	void DrawBlend(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawOrtho(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawPseudo3D(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;

protected:
	Texture* m_tex;

	sm::i16_rect m_quad;
	bool          m_rotate;
	sm::vec2	  m_offset;

	sm::rect m_size;

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_