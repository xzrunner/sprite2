#ifndef _SPRITE2_TYPEDEF_H_
#define _SPRITE2_TYPEDEF_H_

#include <memory>

namespace s2
{

class Symbol;
class Sprite;
class Actor;

typedef std::shared_ptr<Symbol>       SymPtr;
typedef std::shared_ptr<const Symbol> SymConstPtr;

typedef std::shared_ptr<Sprite>       SprPtr;
typedef std::shared_ptr<const Sprite> SprConstPtr;

typedef std::shared_ptr<Actor>        ActorPtr;
typedef std::shared_ptr<const Actor>  ActorConstPtr;

}

#endif // _SPRITE2_TYPEDEF_H_