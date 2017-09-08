#ifndef _SPRITE2_COOKING_H_
#define _SPRITE2_COOKING_H_

#include <CU_Singleton.h>

namespace mt { class Task; }

namespace s2
{

class Sprite;
class RenderParams;

class Cooking
{
public:	
	void Draw(const Sprite* spr, RenderParams* rp);

private:
	SINGLETON_DECLARATION(Cooking)

}; // Cooking

}

#endif // _SPRITE2_COOKING_H_