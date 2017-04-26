#ifndef _SPRITE2_PROXY_HELPER_H_
#define _SPRITE2_PROXY_HELPER_H_

#include <vector>

namespace s2
{

class Sprite;
class Actor;

class ProxyHelper
{
public:
	static Sprite* BuildGroup(const std::vector<Sprite*>& sprs, const Actor* actor);

}; // ProxyHelper

}

#endif // _SPRITE2_PROXY_HELPER_H_
