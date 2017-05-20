#ifndef _SPRITE2_DRAW_DOWNSAMPLE_H_
#define _SPRITE2_DRAW_DOWNSAMPLE_H_

namespace s2
{

class Sprite;
class RenderParams;

class DrawDownsample
{
public:
	static void Draw(const Sprite* spr, const RenderParams& rp, float downsample);

private:
	static void DrawSpr2RT(const Sprite* spr, const RenderParams& rp, float downsample);
	static void DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp, float downsample);

}; // DrawDownsample

}

#endif // _SPRITE2_DRAW_DOWNSAMPLE_H_