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
	static Sprite* BuildGroup(const std::vector<std::pair<const Actor*, Sprite*> >& items);

}; // ProxyHelper

}

#endif // _SPRITE2_PROXY_HELPER_H_
