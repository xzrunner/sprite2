#ifndef _SPRITE2_DRAW_RT_H_
#define _SPRITE2_DRAW_RT_H_

#include <stdint.h>

#include <string>

namespace s2
{

class Sprite;
class Symbol;
class Shape;
class RenderTarget;

class DrawRT
{
public:
	DrawRT();
	DrawRT(int width, int height);
	~DrawRT();

	void Draw(const Sprite* spr, bool clear = false, int width = -1, int height = -1, float dx = 0, float dy = 0, float scale = 1);
	void Draw(const Symbol* sym, bool whitebg = false, float scale = 1.0f);
	void Draw(const Shape* shape, bool clear = false, int width = -1, int height = -1);

	uint8_t* StoreToMemory(int width = -1, int height = -1, int channels = 4);
	void StoreToFile(const std::string& filepath, int width = -1, int height = -1, int channels = 4);

private:
	bool m_rt_reuse;
	RenderTarget* m_rt;

}; // DrawRT

}

#endif // _SPRITE2_DRAW_RT_H_